#include "RecommenderSystem.h"
#define MIN -2

// see documentation on h file
sp_movie RecommenderSystem::add_movie(const std::string& name,int year,const
std::vector<double>& features)
{
  std::shared_ptr<Movie> ptr = std::make_shared<Movie> (name, year);
  _data_base[ptr] = features;
  return ptr;
}

// see documentation on h file
sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
  auto element = _data_base.find(std::make_shared<Movie> (name, year));
  if (element == _data_base.end())
  {
    return nullptr;
  }
  else
  {
    return element->first;
  }
}

// see documentation on h file
std::ostream& operator<<(std::ostream& s, const RecommenderSystem&
recommender_system)
{
  for (const auto& element : recommender_system._data_base)
  {
    s << *(element.first);
  }
  return s;
}

double RecommenderSystem::user_average (const RSUser& user) const
{
  rank_map user_rank = user.get_ranks();
  double sum = 0;
  for(const auto& element : user_rank)
  {
      sum += element.second;
  }
  return sum/(double)user_rank.size();
}


std::vector<sp_movie> RecommenderSystem::get_unranked_movies_vec(rank_map
const& rank_map1) const
{
  std::vector<sp_movie> result;
  for (const auto& elem : _data_base)
  {
    auto current = rank_map1.find (elem.first);
    if (current == rank_map1.end())//then it's not in rank_map- it's not
      // ranked so need to be added
    {
      result.push_back (elem.first);
    }
  }
  return result;
}


std::vector<double> RecommenderSystem::multiply_vector_by_number
(const std::vector<double>& features, double_t num) const
{
  std::vector<double> result(features.size());
  int i = 0;
  for (double elem : features)
  {
    result[i] = (elem * num);
    i++;
  }
  return result;
}

// assuming vectors have the same size
std::vector<double> RecommenderSystem::summing_vectors
    (std::vector<double> const& vec1, std::vector<double> const& vec2) const
{
  std::vector<double> result(vec1.size());
  for (std::vector<double>::size_type i = 0; i < vec1.size(); i++)
  {
    result[i] = vec1[i] + vec2[i];
  }
  return result;
}

// multiplying every vector in _data_base which represent features of a
// movie that has been ranked by given user (which is on rank_map1) and
// summing all of this vector.
// see documentation in h file
std::vector<double> RecommenderSystem::create_favorite_vector(rank_map const&
rank_map1, double average) const
{
  std::vector<double> favorite_vector(_data_base.begin()->second.size(), 0);
  for (const auto& elem : rank_map1)
  {
    auto result = multiply_vector_by_number (_data_base.find (elem.first)->
        second, elem.second-average);
    favorite_vector = summing_vectors(favorite_vector, result);
  }
  return favorite_vector;
}

// α · β = α1 · β1 + ... + αn · βn
// assuming vectors are in the same size


double RecommenderSystem::multiply_vectors(std::vector<double>
    const& vec1, std::vector<double> const& vec2) const
{
  double result = 0;
  for (std::vector<double>::size_type i=0; i < vec1.size(); i++)
  {
    result += vec1[i] * vec2[i];
  }
  return result;
}

double RecommenderSystem::calculate_norma(std::vector<double> const& vec) const
{
  return sqrt (multiply_vectors (vec, vec));
}


double RecommenderSystem::calculate_imagination(std::vector<double> const&
    vec1,std::vector<double> const& vec2) const
{
  return (multiply_vectors (vec1, vec2) / (calculate_norma (vec1) *
  calculate_norma (vec2)));
  // don't need to make sure not dividing by zero because we can
  // assume vectors != 0
}


sp_movie RecommenderSystem::recommend_by_content(const RSUser& user) const
{
  double average = user_average (user);
  // create user favorite vector
  std::vector<double> favorite_vec = create_favorite_vector (user.get_ranks(),
                                                             average);
  auto unranked_movies = get_unranked_movies_vec(user.get_ranks
      ());
  // calculate the imaginary between user favorite features to features of
  // movies user didn't see yet
  double max = MIN;
  sp_movie recommend;
  for (const auto& elem : unranked_movies)
  {
    double current_imaginary = calculate_imagination(favorite_vec,
                                                     _data_base.find
                                                     (elem)->second);
    if (current_imaginary > max)
    {
      max = current_imaginary;
      recommend = elem;
    }
  }
  return recommend;
}

/**
    * Predict a user rating for a movie given argument using item
    * cf procedure with k most similar movies.
    * @param user_rankings: ranking to use
    * @param movie: movie to predict
    * @param k:
    *
    * @return score based on algorithm as described in pdf
    */
double RecommenderSystem::predict_movie_score(const RSUser &user, const
sp_movie &movie, int k) const
{
  // create a set of all the movies' user ranked
  std::map<sp_movie, double> options;
  double current_imaginary;
  for (const auto& elem : user.get_ranks())
  {
    current_imaginary = calculate_imagination((_data_base.find(movie))->second,
                                              (_data_base.find(elem
                                                    .first))->second);
    options[elem.first] = current_imaginary;
  }
  // find the k'th element most similar that have been ranked
  double min;
  for (std::vector<double>::size_type i = 0; i < (options.size()-k); i++)
  {
    min = options.begin()->second;
    sp_movie min_current;
    for (const auto &elem: options)
    {
      if (elem.second < min)
      {
        min = elem.second;
        min_current = elem.first;
      }
    }
    options.erase (min_current);
  }
  // now have in options only the k most similar instances. start level 2
  rank_map r(0, sp_movie_hash, sp_movie_equal);
  double upper_sum = 0;
  double lower_sum = 0;
  for (const auto& elem : options)
  {
    r = user.get_ranks();
    upper_sum += elem.second * (r.find(elem.first))->second;
    lower_sum += elem.second;
  }
  return upper_sum/lower_sum; //cen assume lower_sum is not zero because
  // the vectors != 0 so the calculation will be != 0
}

// see documentation in h file
sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k) const
{
  // for every movie the user didn't see, find it's score. at last, return
  // the movie that got the highest predicted ranking
  auto movies_to_rank = get_unranked_movies_vec(user.get_ranks());
  double max_score = 0; //ok?
  double current_score;
  sp_movie recommend_movie;
  for (const auto& elem : movies_to_rank)
  {
    current_score = predict_movie_score (user, elem, k);
    if (current_score > max_score)
    {
      max_score = current_score;
      recommend_movie = elem;
    }
  }
  return recommend_movie;
}

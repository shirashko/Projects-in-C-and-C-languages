
// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"


// see documentation in h file
RSUser::RSUser(const std::string& name, const rank_map& rank_map, const
std::shared_ptr<RecommenderSystem>& recommender_system)
{
  _user_name = name;
  _rank_map = rank_map;
  _recommender_system = recommender_system;
}

// see documentation in h file
const std::string& RSUser::get_name() const
{
  return _user_name;
}

// see documentation in h file
void RSUser::add_movie_to_rs(const std::string &name, int year, const
std::vector<double> &features, double rate)
{
  // adding to _recommender_system , and adding it to rank_map
  _recommender_system->add_movie (name,year,features);
  _rank_map[_recommender_system->get_movie (name, year)] = rate;
}

// see documentation in h file
const rank_map& RSUser::get_ranks() const
{
  return _rank_map;
}

// see documentation in h file
sp_movie RSUser::get_recommendation_by_cf(int k) const
{
  return _recommender_system->recommend_by_cf (*this, k);
}

// see documentation in h file
sp_movie RSUser::get_recommendation_by_content() const
{
  return _recommender_system->recommend_by_content(*this);
}

// see documentation in h file
double RSUser::get_prediction_score_for_movie(const std::string& name, int
year, int k) const
{
  // do I need to make sure this movie is in the _data_base of
  // _recommender_system?
  return _recommender_system->predict_movie_score(*this,
                                                  _recommender_system->
                                                  get_movie (name,year) ,k);
}

// see documentation in h file
std::ostream& operator<<(std::ostream& s, const RSUser& user)
{
  s << "name: " << user._user_name << std::endl;
  // output all the movies in _recommender_system
  s << *user._recommender_system;
  return s;
}

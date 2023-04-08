
#include "RSUsersLoader.h"
#include <fstream>
#define FILE_ERROR "Failed to open file"
#include <sstream>
#define DELIM '-'
#define NOT_RANKED "NA"
#define MIN_SCORE 1
#define MAX_SCORE 10
#define RANK_OUT_OF_RANGE "movie rank value is not in the right range"


std::vector<sp_movie> RSUsersLoader::create_movies_vec(const std::string&
movies_buffer)
{
  std::istringstream iss (movies_buffer); // making what holding first line
  // to a stream
  std::string movie_name; // hold current movie name
  int movie_year; // hold current movie year
  std::string movie_info;
  std::vector<sp_movie> file_movies_vec; //vector to hold pointers to Movies
  // getting information about the movies from the file
  while (iss)
  {
    iss >> movie_info;
    std::istringstream iss1 (movie_info);
    getline (iss1, movie_name, DELIM); //getting current movie name
    iss1 >> movie_year; //getting current movie year
    file_movies_vec.push_back (std::make_shared<Movie> (movie_name,
                                                        movie_year)); //ok?
  }
  return file_movies_vec;
}

void RSUsersLoader::add_user_to_vec(const std::vector<sp_movie>&
    file_movies_vec, const std::string& user_buffer, const
    std::shared_ptr<RecommenderSystem>& recommender_system,
std::vector<RSUser>& users_vec)
{
  //user_buffer contain the information about current user
  std::istringstream iss1 (user_buffer); // user_buffer is now a stream
  std::string user_name;
  iss1 >> user_name;
  // creating _rank_map=ranking_map for current user, with functions
  // sp_movie_hash , sp_movie_equal implemented for us
  rank_map ranking_map(0, sp_movie_hash, sp_movie_equal);
  std::string current_rank;
  double rank;
  for (const auto& i: file_movies_vec)
  {
    iss1 >> current_rank;
    if (current_rank != NOT_RANKED)
    {
      std::istringstream iss2 (current_rank);
      iss2 >> rank;
      if (rank < MIN_SCORE || rank > MAX_SCORE) //invalid argument
      {
        throw std::invalid_argument (RANK_OUT_OF_RANGE);
      }
      ranking_map[i] = rank;
    }
  } // now _rank_map of the current user exist
  RSUser current_user (user_name, ranking_map, recommender_system);
  users_vec.push_back (current_user);
}


// see full documentation in h file
std::vector<RSUser> RSUsersLoader::create_users_from_file(const std::string&
users_file_path, std::unique_ptr<RecommenderSystem> rs)  noexcept (false)
{
  // opening file and read from it
  std::shared_ptr<RecommenderSystem> recommender_system = std::move (rs);
  // open given file
  std::ifstream input_file (users_file_path);
  if (input_file.fail ())
  {
    throw std::runtime_error (FILE_ERROR);
  }
  // taking care of first line, which is for movies names and movies years
  std::string movies_buffer;
  getline (input_file, movies_buffer); //now movies_buffer hold first line
  // from the file
  std::vector<sp_movie> file_movies_vec = create_movies_vec(movies_buffer);
  // now file_movies_vec contain all relevant movies to ranks
  // now for each line in the input_file create a user
  std::string user_buffer;
  std::vector<RSUser> users_vec;
  while (getline (input_file, user_buffer))
  {
    add_user_to_vec (file_movies_vec, user_buffer, recommender_system,
                     users_vec);
  }
  return users_vec;
}

#include "RecommenderSystemLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#define MIN_SCORE 1
#define MAX_SCORE 10
#define FILE_ERROR "Failed to open file"
#define DELIM '-'
#define FEATURE_OUT_OF_RANGE "movie feature value is not in the right range"

// see documentation in h file
std::unique_ptr<RecommenderSystem>
    RecommenderSystemLoader::create_rs_from_movies_file(const std::string&
movies_file_path)
{
  // open given file and read from it
  std::ifstream movies_file (movies_file_path);
  if (movies_file.fail ())
  {
    throw std::runtime_error (FILE_ERROR);
  }
  std::string buffer;
  auto recommender_system_ptr = std::make_unique<RecommenderSystem> ();
  while (getline (movies_file, buffer))
  {
    std::istringstream iss (buffer);
    std::string movie_name;
    std::getline (iss, movie_name, DELIM);
    int movie_year;
    iss >> movie_year;
    double feature_value;
    std::vector<double> features_values = std::vector<double> ();
    while (iss >> feature_value)
    {
      //check value is in the valid range
      if (feature_value < MIN_SCORE || feature_value > MAX_SCORE)
      {
        //feature_value is not in the valid range
        throw std::invalid_argument (FEATURE_OUT_OF_RANGE);
      }
      features_values.push_back (feature_value);
    }
    // now I have a movie_name, movie_year , and features_values, to enter
    // to the RecommenderSystem instance
    recommender_system_ptr->add_movie (movie_name, movie_year,
                                       features_values);
  }
  return recommender_system_ptr;
}
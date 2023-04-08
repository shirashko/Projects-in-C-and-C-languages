
#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>
#include <cmath>

struct compare_movies
{
  bool operator()(const sp_movie& m1, const sp_movie& m2) const
  {
    return (*m1) < (*m2);
  }
};

typedef std::map<sp_movie, std::vector<double>, compare_movies> data_base;


/**
 * represent a System which which users connected to, and it recommend them
 * on movies which is on the system based on their features
 */
class RecommenderSystem
{
 private:
    data_base _data_base;

    double user_average(const RSUser& user) const;
    std::vector<sp_movie> get_unranked_movies_vec(rank_map const& rank_map1)
    const;
    std::vector<double> create_favorite_vector(rank_map const& rank_map1,
                                                double average) const;
    std::vector<double> multiply_vector_by_number(std::vector<double>
        const& features, double num) const;
    std::vector<double> summing_vectors (std::vector<double> const&
        vec1, std::vector<double> const& vec2) const;
    double multiply_vectors(std::vector<double> const& vec1,
                                       std::vector<double> const& vec2) const;
    double calculate_norma(std::vector<double> const& vec) const;
    double calculate_imagination(std::vector<double> const& vec1,
                               std::vector<double> const& vec2) const;

 public:

    /**
     * construct RecommenderSystem instance
     */
	explicit RecommenderSystem() = default;

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,const
    std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user) const;

    /**
     * a function that calculates the movie with highest predicted score
     * based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k) const;

    /**
     * Predict a user rating for a movie given argument using item cf procedure
     * with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie& movie,
												  int k) const;

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string& name, int year) const;

  /**
   * output to the stream all of the movies in the _data_base in sorted order
   * according to < operator of Movie class
   */
	friend std::ostream& operator<<(std::ostream& s, const RecommenderSystem&
    recommender_system);

};

#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
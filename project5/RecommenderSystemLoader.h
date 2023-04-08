
#ifndef EX5_RECOMMENDERSYSTEMLOADER_H
#define EX5_RECOMMENDERSYSTEMLOADER_H


#include "RecommenderSystem.h"


class RecommenderSystemLoader
{
public:
    RecommenderSystemLoader() = delete;
    /**
	 * loads movies by the given format for movies with their feature's score
	 * @param movies_file_path a path to the file of the movies
	 * @return unique pointer to a RecommenderSystem which was created with
     * those movies
	 */
    static std::unique_ptr<RecommenderSystem> create_rs_from_movies_file(const
    std::string& movies_file_path) noexcept(false);
};


#endif //EX5_RECOMMENDERSYSTEMLOADER_H

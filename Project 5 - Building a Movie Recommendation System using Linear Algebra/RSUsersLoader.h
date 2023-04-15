
#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H
#include "RSUser.h"
#include "RecommenderSystem.h"

class RSUsersLoader
{
private:
  static std::vector<sp_movie> create_movies_vec(const std::string&
  movies_buffer);
  static void add_user_to_vec(const std::vector<sp_movie>& file_movies_vec,
                              const std::string& user_buffer, const
                       std::shared_ptr<RecommenderSystem>& recommender_system,
                       std::vector<RSUser>& users_vec);
public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their movie
     * ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file(const std::string&
    users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept
    (false);
};

#endif //SCHOOL_SOLUTION_USERFACTORY_H

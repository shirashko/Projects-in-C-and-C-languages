#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

#include "HashMap.hpp"
#define DEFAULT_MESSAGE ""

using std::string;



class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey() : std::invalid_argument (DEFAULT_MESSAGE) {};
  explicit InvalidKey(const string& s) : std::invalid_argument (s) {};
};

/**
 * a container of pairs of strings
 */
class Dictionary : public HashMap<string,string>
{
 public:
  using HashMap<string, string> :: HashMap;

  /**
   * erase the pair with the given key from this HashMap if it has this key.
   * if this HashMap doesn't contain given key - throws error
   * @param key
   * @return true if the value was successfully remove
   */
  bool erase(const string& key) override  // to write override is ok?
  {
    if (! HashMap<string, string>::erase (key))
    {
      throw InvalidKey(ERROR_MESSAGE);
    }
    return true;
  }

  /**
   * get iterator to the start and an iterator to the end of a range of
   * items to add to this Dictionary
   * assuming the given iterators are forward iterators containing items
   * from type std::pair<std::string, std::string>
   * @param first - indicate the start of the range
   * @param last - indicate the end of the range
   */
  template<class Iter>
  void update(Iter first, Iter last)
  {
      for (auto i = first; i != last; i++)
      {
        (*this)[(*i).first] = (*i).second;
      }
  }
};



#endif //_DICTIONARY_HPP_

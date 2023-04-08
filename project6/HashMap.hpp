#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include <algorithm>
#include <vector>
#include <stdexcept>
#define INITIAL_SIZE 0
#define INITIAL_CAPACITY 16
#define EMPTY 0
#define MAX_LOAD_FACTOR 0.75
#define MIN_LOAD_FACTOR 0.25
#define BASE 2
#define END -1
#define ERROR_MESSAGE "invalid argument"

/**
 * Generic HashMap container of keys and values
 * assuming KeyT support std::hash, keyT and ValueT support operator=,
 * operator==, and that both of them have default constructor
 * @tparam KeyT represent the keys type in this HashMap
 * @tparam ValueT represent the values type in this HashMap
 */
template <class KeyT, class ValueT>
class HashMap
{
 protected:
  typedef std::vector<std::pair<KeyT, ValueT>> bucket;
  bucket* _map;
  int _capacity;
  int _size;

  // calculating a matching index to insert pair with the given key
  int hush_function(const KeyT& key, int capacity) const
  {
      return (std::hash<KeyT> {} (key)) & (capacity-1);
  }

  // change size of _map due to passing load factor minimum / maximum valid
  // range. recalculate the hash values for every key and placing them in
  // the suitable position
  void rehashing()
  {
    int capacity = _capacity;
    if (capacity == 0) { capacity = 1;}
    if (_size == 0) { _capacity = 1; return; }
    if (MIN_LOAD_FACTOR > get_load_factor ())
    {
      while( ((double)(_size)/capacity) < MIN_LOAD_FACTOR )
      {
        capacity /= BASE;
      }
    }
    else
    {
      while( ((double)(_size)/capacity) > MAX_LOAD_FACTOR )
      {
        capacity *= BASE;
      }
    }
    auto map = new bucket[capacity];
    for (std::pair<KeyT, ValueT> current: (*this))
    {
      bucket& cur_bucket = map[hush_function (current.first, capacity)];
      cur_bucket.push_back (current);
    }
    delete[] _map;
    _capacity = capacity;
    _map = map;
  }

 public:

  /**
   * default instructor
   */
  HashMap() : _map(new bucket[INITIAL_CAPACITY]), _capacity(INITIAL_CAPACITY),
  _size(INITIAL_SIZE) {};


  /**
   * construct HashMap contains pairs according to given keys and values in
   * matching places. throw error if given number of keys is different from
   * given number of values
   * @param keys
   * @param values
   */
  HashMap(const std::vector<KeyT>& keys, const std::vector<ValueT>& values)
  {
    _size = EMPTY;
    if (keys.size () != values.size ())
    {
      throw std::length_error (ERROR_MESSAGE);
    }
    _capacity = INITIAL_CAPACITY;
    _map = new bucket[_capacity];
    // insert pairs to map
    for (int i = 0; i < (int)keys.size(); i++)
    {
      (*this)[keys[i]] = values[i]; // [] support insert without duplication
    }
  }


  /**
   * copy constructor
   * @param other HashMap to (deep) copy from
   */
  HashMap(const HashMap<KeyT,ValueT>& other) : _map(new bucket[other
  ._capacity]), _capacity(other._capacity) , _size(other._size)
  {
    // inserting values from other _map to this _map
    for (int i=0; i < _capacity; i++)
    {
      _map[i] = other._map[i];
    }
  }

  /**
   * destructor
   */
   virtual ~HashMap()
  {
    delete [] _map;
    //vectors destruct them selves
  }

  /**
   * @return number of elements in this HashMap
   */
  int size() const
  {
    return _size;
  }

  /**
   * @return number of buckets in this HashMap that a given key can be mapped
   * into
   */
  int capacity() const
  {
    return _capacity;
  }

  /**
   * @return true if empty (doesn't contain elements) and false if not empty
   */
  bool empty() const
  {
    return (_size == EMPTY);
  }

  /**
   * inserting a key and a value as a pair to HashMap
   * if key is already in the HashMap - do nothing
   * @param key
   * @param value
   * @return true if key is not in the HashMap and added by success, false
   * otherwise
   */
  bool insert(const KeyT& key,const ValueT& value)
  {
      if (contains_key (key))
      { //key is already in HashMap
        return false;
      }
      bucket& cur_bucket = _map[hush_function (key,_capacity)];
      cur_bucket.emplace_back(key, value); //add to the end
      _size++;
      // need to check the load factor and to change if necessary...
      if (get_load_factor() > MAX_LOAD_FACTOR)
      {
        rehashing();
      }
      return true;
  }

  /**
   *
   * @param key
   * @return true if key is in this HashMap, false otherwise
   */
  bool contains_key(const KeyT& key) const
  {
    // looping on the bucket where key can potentially be
    bucket& cur_bucket = _map[hush_function(key, _capacity)];
    for (int i =0; i < (int)cur_bucket.size(); i++)
    {
      if (cur_bucket[i].first == key)
      {
        return true;
      }
    }
    return false;
  }


  /**
   * if key is not in the HashMap, insert it with default value
   * @param key - the info needed to find the value associated with
   * @return the value of given key if exist or the default value of ValueT
   * if key wasn't in the HashMap
   */
  ValueT& operator[](const KeyT& key)
  {
    if (contains_key (key))
    {
      bucket &cur_bucket = _map[bucket_index (key)];
      for (int i = 0; i < (int)cur_bucket.size (); i++)
      {
        if (cur_bucket[i].first == key)
        {
          return cur_bucket[i].second;
        }
      }
    }
    // if got here - key isn't in HashMap
    insert(key, ValueT());
    bucket& cur_bucket = _map[hush_function(key,_capacity)];
    return (cur_bucket[bucket_size(key)-1]).second;
  }

  /**
   * if key is not in the HashMap, insert it with default value
   * @param key - the info needed to find the value associated with
   * @return the value of given key if exist or the default value of ValueT
   * if key wasn't in the HashMap
   */
  const ValueT& operator[](const KeyT& key) const
  {
    if (contains_key (key))
    {
      bucket& cur_bucket = _map[bucket_index(key)];
      for (int i =0; i < (int)cur_bucket.size(); i++)
      {
        if (cur_bucket[i].first == key)
        {
          return cur_bucket[i].second;
        }
      }
    }
    bucket& cur_bucket = _map[hush_function (key,_capacity)];
    return (cur_bucket[bucket_size(key)-1]).second;
  }

  /**
   * throw error in case of key is not in this HashMap
   * @param key - the key to find it's value in this HashMap
   * @return the value of this key in this HashMap
   */
  ValueT& at(const KeyT& key)
  {
    if (!contains_key(key))
    {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    return ((*this)[key]);
  }

  /**
   *
   * @param key
   * @return
   */
  const ValueT& at(const KeyT& key) const
  {
    if (!contains_key(key))
    {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    return ((*this)[key]);
  }

  /**
   * erase the pair with this key from this HashMap if it has this key
   * @param key
   * @return true if the value was successfully remove, false otherwise
   */
  virtual bool erase(const KeyT& key)
  {
      if (!contains_key (key))
      {
        return false;
      }
        bucket &cur_bucket = _map[hush_function (key, _capacity)];
        for (int i = 0; i < (int)cur_bucket.size (); i++)
        {
          if (cur_bucket[i].first == key)
          {
            cur_bucket.erase (cur_bucket.begin () + i);
            _size--;
            // need to check the load factor and to change if necessary...
            if (get_load_factor () < MIN_LOAD_FACTOR)
            {
              rehashing ();
            }
            return true;
          }
        }
    return false; // problem
  }

  /**
   * calculate the proportion between number of elements in this HashMap and
   * it's capacity
   * @return load factor - the proportion between number of elements in this
   * HashMap and it's capacity
   */
  double get_load_factor() const
  {
    return ((double) _size / _capacity); //capacity always != 0
  }
  /**
   * return the number of elements sharing the bucket in HashMap with key
   * throw error if key is not in this HashMap
   * @param key - the key to look for it's bucket index
   * @return the number of elements sharing the bucket in HashMap with key
   */
  int bucket_size(const KeyT& key) const
  {
    {
      if (!contains_key (key))
      {
        throw std::invalid_argument(ERROR_MESSAGE);
      }
      bucket& cur_bucket = _map[hush_function(key,_capacity)];
      return (int)cur_bucket.size();
    }
  }

  /**
   * return the index of the bucket given key is inside of. throw error if key
   * is not in this HashMap.
   * @param key - the key to look for it's bucket index
   * @return the index of the bucket given key is inside of
   */
  int bucket_index(const KeyT& key) const
  {
    if (!contains_key (key))
    {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    return hush_function(key, _capacity);
  }

  /**
   * remove all the items (pairs of keys and values) in HashMap, without
   * changing the capacity
   */
  void clear()
  {
    for (int i=0; i < _capacity; i++)
    {
      bucket& cur_bucket = _map[i];
      cur_bucket.erase(cur_bucket.begin(), cur_bucket.end());
    }
    _size = EMPTY;
  }

  /**
   * changing this HashMap to be equal (deep copy) in values to other given
   * HashMap
   * @param other - given HashMap to compare this HashMap with
   * @return this HashMap after the change
   */
  HashMap<KeyT,ValueT>& operator=(const HashMap<KeyT,ValueT>& other)
  {
    if (this == &other)
    {
      return *this;
    }
    delete[] _map;
    _capacity = other._capacity , _size = other._size;
    _map = new bucket[_capacity];
    // insert values from other _map to this _map
    for (int i = 0; i < _capacity; i++)
    {
      bucket& cur_bucket = _map[i];
      cur_bucket = other._map[i]; // make sure it's deep copy
    }
    return *this;
  }

  /**
   * check if this HashMap contain the same elements as other HashMap object
   * @param other HashMap to compare with
   * @return true if HashMap contains the same values, false otherwise
   */
  bool operator==(const HashMap<KeyT,ValueT>& other) const
  {
    if (_size != other._size)
    {
      return false;
    }
    for (std::pair<KeyT,ValueT> current : *this) //using HashMap iterator
    {
      if (!other.contains_key (current.first) || other[current.first] !=
      current.second)
      {
        return false;
      }
    }
    return true;
  }

  /**
   * check if this HashMap contain different elements as other HashMap object
   * @param other HashMap to compare with
   * @return false if HashMap contains the same values, true otherwise
   */
  bool operator!=(const HashMap<KeyT,ValueT>& other) const
  {
    return ! ((*this) == other);
  }


  /**
   * const iterator of HashMap
   */
  class Const_Iterator;

  typedef Const_Iterator const_iterator;

  /**
   * get iterator to the first element in the HashMap
   * @return iterator to the first element in the HashMap
   */
  const_iterator begin() const
  {
    Const_Iterator iter(this);
    return iter;
  }

  /**
    * get iterator to the first element in the HashMap
    * @return iterator to the first element in the HashMap
   */
  const_iterator cbegin() const
  {
    return begin();
  }

    /**
   * get iterator to the last element in the HashMap
   * @return iterator to the last element in the HashMap
   */
  const_iterator end() const
  {
    Const_Iterator iter(this);
    for (int i=0; i < _size; i++)
    {
      ++iter;
    }
    return iter;
  }

  /**
 * get iterator to the last element in the HashMap
 * @return iterator to the last element in the HashMap
 */
  const_iterator cend() const
  {
    return end();
  }
};

template <class KeyT, class ValueT>
class HashMap<KeyT,ValueT>::Const_Iterator
{
 protected:
  const HashMap<KeyT,ValueT>* _hash_map;
  int _bucket_index;
  int _index_of_pair_in_bucket;

 public:
  typedef std::pair<KeyT,ValueT> value_type;
  typedef const std::pair<KeyT,ValueT>& reference;
  typedef const std::pair<KeyT,ValueT>* pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::forward_iterator_tag iterator_category;

  /**
   * construct a const iterator
   * @param hash_map - the HashMap to iterate on
   */
  Const_Iterator(const HashMap<KeyT,ValueT>* hash_map) : _hash_map(hash_map)
  {
    if (hash_map->empty())
    {
      _bucket_index = END;
      _index_of_pair_in_bucket = END;
    }
    // check which is the first bucket there is a pair in
    for (int i = 0; i < hash_map->_capacity; i++)
    {
      bucket& cur_bucket = hash_map->_map[i];
      if (!cur_bucket.empty ())
      {
        _bucket_index = i;
        _index_of_pair_in_bucket = INITIAL_SIZE;
        break;
      }
    }
  }

  /**
   * promote the iterator that contain next element in HashMap
   * @return the iterator called the function
   */
  Const_Iterator operator++(int) // Postfix increment
  {
    Const_Iterator temp(*this);
    bucket& cur_bucket = _hash_map->_map[_bucket_index];
    if ((int)cur_bucket.size() == (_index_of_pair_in_bucket+1)) // true ->
    // search in next bucket
    {
      for (int i = _bucket_index+1; i < _hash_map->_capacity; i++)
      {
        bucket& cur_bucket = _hash_map->_map[i];
        if (!cur_bucket.empty())
        {
          _bucket_index = i;
          _index_of_pair_in_bucket = INITIAL_SIZE;
          return temp;
        }
      }
      // if got here -> we are in the last element and now ++ -> iterator.end()
      _bucket_index = END;
      _index_of_pair_in_bucket = END;
    }
    else // there is still element in current bucket
    {
      _index_of_pair_in_bucket++;
    }
    return temp;
  }

  /**
   * get the iterator that contain next element in HashMap
   * @return  iterator to the next element in HashMap
   */
  Const_Iterator& operator++() // Preincrement
  {
    (*this)++;
    return *this;
  }

  /**
   * compare if two iterator are the same
   * @param other the iterator to compare with this
   * @return true if equal, false otherwise
   */
  bool operator==(const Const_Iterator& other) const
  {
    return _hash_map == other._hash_map && _bucket_index == other
    ._bucket_index && _index_of_pair_in_bucket == other
    ._index_of_pair_in_bucket;
  }

  /**
   * check if two iterators are equal
   * @param other - iterator to compare with this
   * @return true if equals, false otherwise
   */
  bool operator!=(const Const_Iterator& other) const
  {
    return ! ((*this) == other);
  }

  /**
   * the pair of this iterator
   * @return the pair of this iterator
   */
  reference operator*() const
  {
    return _hash_map->_map[_bucket_index][_index_of_pair_in_bucket];
  }

  /**
   * a pointer to the pair of this iterator
   * @return a pointer to the pair of this iterator
   */
  pointer operator->() const
  {
    if (_bucket_index == END && _index_of_pair_in_bucket == END)
    {
      return nullptr;
    }
    return &(_hash_map->_map[_bucket_index][_index_of_pair_in_bucket]);
  }
};


#endif //_HASHMAP_HPP_

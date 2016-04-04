#ifndef P4C_LIB_ORDERED_SET_H_
#define P4C_LIB_ORDERED_SET_H_

#include <functional>
#include <list>
#include <set>
#include <utility>

template <class T, class COMP = std::less<T>, class ALLOC = std::allocator<T>>
class ordered_set {
 public:
    typedef T                   key_type;
    typedef T                   value_type;
    typedef COMP                key_compare;
    typedef COMP                value_compare;
    typedef ALLOC               allocator_type;
    typedef const T             &reference;
    typedef const T             &const_reference;

 private:
    typedef std::list<T, ALLOC> list_type;
    list_type                   data;

 public:
    typedef typename list_type::iterator                iterator;
    typedef typename list_type::const_iterator          const_iterator;
    typedef std::reverse_iterator<iterator>             reverse_iterator;
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

 private:
    struct mapcmp : std::binary_function<const T *, const T *, bool> {
        COMP    comp;
        bool operator()(const T *a, const T *b) const { return comp(*a, *b); } };
    typedef std::map<const T *, iterator, mapcmp, ALLOC>  map_type;
    map_type                            data_map;
    void init_data_map() {
        data_map.clear();
        for (auto it = data.begin(); it != data.end(); it++)
            data_map.emplace(&*it, it); }
    iterator tr_iter(typename map_type::iterator i) {
        if (i == data_map.end())
            return data.end();
        return i->second; }
    const_iterator tr_iter(typename map_type::const_iterator i) const {
        if (i == data_map.end())
            return data.end();
        return i->second; }

 public:
    typedef typename map_type::size_type        size_type;

    ordered_set() {}
    ordered_set(const ordered_set &a) : data(a.data) { init_data_map(); }
    ordered_set(ordered_set &&a) = default; /* move is ok? */
    ordered_set &operator=(const ordered_set &a) { data = a.data; init_data_map(); }
    ordered_set &operator=(ordered_set &&a) = default; /* move is ok? */
    // FIXME add allocator and comparator ctors...

    iterator                    begin() noexcept { return data.begin(); }
    const_iterator              begin() const noexcept { return data.begin(); }
    iterator                    end() noexcept { return data.end(); }
    const_iterator              end() const noexcept { return data.end(); }
    reverse_iterator            rbegin() noexcept { return data.rbegin(); }
    const_reverse_iterator      rbegin() const noexcept { return data.rbegin(); }
    reverse_iterator            rend() noexcept { return data.rend(); }
    const_reverse_iterator      rend() const noexcept { return data.rend(); }
    const_iterator              cbegin() const noexcept { return data.cbegin(); }
    const_iterator              cend() const noexcept { return data.cend(); }
    const_reverse_iterator      crbegin() const noexcept { return data.crbegin(); }
    const_reverse_iterator      crend() const noexcept { return data.crend(); }

    bool        empty() const noexcept { return data.empty(); }
    size_type   size() const noexcept { return data_map.size(); }
    size_type   max_size() const noexcept { return data_map.max_size(); }

    iterator        find(const T &a) { return tr_iter(data_map.find(&a)); }
    const_iterator  find(const T &a) const { return tr_iter(data_map.find(&a)); }
    size_type       count(const T &a) const { return data_map.count(&a); }
    iterator        upper_bound(const T &a) { return tr_iter(data_map.upper_bound(&a)); }
    const_iterator  upper_bound(const T &a) const { return tr_iter(data_map.upper_bound(&a)); }
    iterator        lower_bound(const T &a) { return tr_iter(data_map.lower_bound(&a)); }
    const_iterator  lower_bound(const T &a) const { return tr_iter(data_map.lower_bound(&a)); }

    std::pair<iterator, bool> insert(const T &v) {
        auto it = find(v);
        if (it == data.end()) {
            it = data.insert(data.end(), v);
            data_map.emplace(&*it, it);
            return std::make_pair(it, true); }
        return std::make_pair(it, false); }
    std::pair<iterator, bool> insert(T &&v) {
        auto it = find(v);
        if (it == data.end()) {
            it = data.insert(data.end(), std::move(v));
            data_map.emplace(&*it, it);
            return std::make_pair(it, true); }
        return std::make_pair(it, false); }

    template <class... Args>
    std::pair<iterator, bool> emplace(Args &&... args) {
        auto it = data.emplace(data.end(), std::forward<Args>(args)...);
        auto old = find(*it);
        if (old == data.end()) {
            data_map.emplace(&*it, it);
            return std::make_pair(it, true);
        } else {
            data.erase(it);
            return std::make_pair(old, false); } }

    iterator erase(const_iterator pos) {
        data_map.erase(&*pos);
        return data.erase(pos); }
    size_type erase(const T &v) {
        auto it = find(v);
        if (it != data.end()) {
            data_map.erase(&v);
            data.erase(it);
            return 1; }
        return 0; }
};

#endif /* P4C_LIB_ORDERED_SET_H_ */
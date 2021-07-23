#ifndef RECIPES_BASE_H
#define RECIPES_BASE_H

namespace recipes {
namespace base {
class nocopyable {
 public:
  nocopyable() = default;
  nocopyable(nocopyable& other) = delete;
  nocopyable& operator=(nocopyable& other) = delete;
};

}  // namespace base

}  // namespace recipes

#endif /* RECIPES_BASE_H */
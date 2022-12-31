#ifndef ELEMENTUM_HPP
#define ELEMENTUM_HPP

namespace yuki {

enum class ElementumType { None, Pyro, Hydro, Cyro };
enum class ElementumReactionType { None, Evaporation, Freeze, Melt };

struct Elementum {
  Elementum(ElementumType type = ElementumType::None, float level = 0.0f)
      : type(type), level(level) {}

  ElementumType type;
  float level;
};

struct ElementumReaction {
  ElementumReactionType type;
  Elementum elementum;
};

ElementumReactionType getElementumReactionType(const Elementum& lhs,
                                               const Elementum& rhs);

Elementum getReactedElementum(const Elementum& lhs, const Elementum& rhs);

ElementumReaction getElementumReaction(const Elementum& lhs,
                                       const Elementum& rhs);

}  // namespace yuki

#endif  // ELEMENTUM_HPP

#include "Elementum.hpp"

using namespace yuki;

Elementum yuki::getReactedElementum(const Elementum& lhs, const Elementum& rhs) {
  if (lhs.type == ElementumType::None) {
    return rhs;
  }
  if (lhs.type == rhs.type || rhs.type == ElementumType::None) {
    return lhs;
  }

  Elementum result(lhs);

  switch (lhs.type) {
    case ElementumType::Pyro:
      if (rhs.type == ElementumType::Hydro) {
        result.level -= rhs.level;
        if (result.level < 0) {
          result.type = ElementumType::Hydro;
          result.level = -result.level;
        }
      }
      if (rhs.type == ElementumType::Cyro) {
        result.level -= 2 * rhs.level;
        if (result.level < 0) {
          result.type = ElementumType::Cyro;
          result.level = -result.level;
        }
      }
      break;

    case ElementumType::Hydro:
      if (rhs.type == ElementumType::Pyro) {
        result.level -= rhs.level;
        if (result.level < 0) {
          result.type = ElementumType::Pyro;
          result.level = -result.level;
        }
      }
      if (rhs.type == ElementumType::Cyro) {
        result.type = ElementumType::Cyro;
      }
      break;

    case ElementumType::Cyro:
      if (rhs.type == ElementumType::Pyro) {
        result.level -= 2 * rhs.level;
        if (result.level < 0) {
          result.type = ElementumType::Pyro;
          result.level = -result.level;
        }
      }
      break;
    default:
      break;
  }

  return result;
}

ElementumReactionType yuki::getElementumReactionType(const Elementum& lhs,
                                               const Elementum& rhs) {
  switch (lhs.type) {
    case ElementumType::Pyro:
      if (rhs.type == ElementumType::Hydro) {
        return ElementumReactionType::Evaporation;
      }
      if (rhs.type == ElementumType::Cyro) {
        return ElementumReactionType::Freeze;
      }
      break;
    case ElementumType::Hydro:
      if (rhs.type == ElementumType::Pyro) {
        return ElementumReactionType::Evaporation;
      }
      if (rhs.type == ElementumType::Cyro) {
        return ElementumReactionType::Freeze;
      }
      break;
    case ElementumType::Cyro:
      if (rhs.type == ElementumType::Pyro) {
        return ElementumReactionType::Freeze;
      }
      break;
    default:
      break;
  }

  return ElementumReactionType::None;
}

ElementumReaction yuki::getElementumReaction(const Elementum& lhs,
                                       const Elementum& rhs) {
  ElementumReaction result;
  result.type = getElementumReactionType(lhs, rhs);
  result.elementum = getReactedElementum(lhs, rhs);
  return result;
}

#ifndef SFRP_BEHAVIORMAP_HPP_
#define SFRP_BEHAVIORMAP_HPP_

//@PURPOSE: Provide a means to apply a plain function to behavior objects.
//
//@CLASSES:
//  sfrp::BehaviorMap: apply function to behaviors functor
//
//@DESCRIPTION: This component provides a single functor class, 'BehaviorMap',
// that applies a plain function to behaviors. For instance, if we have a
// function of type '(int,string)->Foo', we can apply that to two behaviors with
// types 'Behavior<int>', and 'Behavior<string>' to produce a 'Behavior<Foo>'
// result.
//
// Semantics
// ---------
// 'BehaviorMap' is a polymorphic function that accepts many numbers of
// arguments. In our semantics below we extend our normal Agda notation with
// elipses to indicate that that is variadic.
//
// The type can be thought of as converting any multiargument function into a
// function acting on behaviors.
//..
//  μ⟦BehaviorMap()⟧ : {α₁ α₂ ⋯ αₙ: Set} → (α₁ → α₂ ⋯ αₙ → β) →
//    ( t : T, f : (0,t) → α₁ ) →
//    ( t : T, f : (0,t) → α₂ ) ⋯
//    ( t : T, f : (0,t) → αₙ ) →
//    ( t : T, f : (0,t) → β )
//..
// The implementation is as follows
//..
//  μ⟦BehaviorMap()⟧ = λ f (t₁,f₁) (t₂,f₂) ⋯ (tₙ,fₙ) →
//    ( minimum( t₁, t₂ ⋯ tₙ), λt → f (f₁ t) (f₂ t) ⋯ (fₙ t) )
//..
//
// 'BehaviorMap' is a Mathematical Functor
// ---------------------------------------
// The behavior map function forms a mathematical functor from the category of
// C++ values to the category of behaviors. We can prove this using the
// semantics of behaviors and the semantics of the map function.
//
// For the first law of functors we must show that the identify function on c++
// values is mapped to the identify function on behaviors.
//..
// μ⟦BehaviorMap()⟧ (λx→x) ≡ (λx→x)
//..
// To prove this we invoke the definition of 'μ⟦BehaviorMap()⟧'.
//..
// μ⟦BehaviorMap()⟧ (λx→x) ≡
// λ (t₁,f₁) → (minimum(t₁), λt → (λx→x) (f₁ t)) ≡
// λ (t₁,f₁) → (t₁, λt → (λx→x) (f₁ t)) ≡
// λ (t₁,f₁) → (t₁, λt → f₁ t) ≡
// λ (t₁,f₁) → (t₁, f₁) ≡
// λx→x
//..
// The second law we need to show is that composition is maintained.
//..
// μ⟦BehaviorMap()⟧ (f ∘ g) ≡ (μ⟦BehaviorMap()⟧ f) ∘ (μ⟦BehaviorMap()⟧ g)
//..
// We first expand the left side of the proof:
//..
// μ⟦BehaviorMap()⟧ (f ∘ g) ≡
// λ (t₁, f₁) → (minimum(t₁), λt → (f ∘ g) (f₁ t)) ≡
// λ (t₁, f₁) → (t₁, λt → f (g (f₁ t))) ≡
//..
// Now we expand the right side of the proof:
//..
// (μ⟦BehaviorMap()⟧ f) ∘ (μ⟦BehaviorMap()⟧ g) ≡
// (λ (t₁,f₁) → (t₁, λt → f (f₁ t))) ∘
//   (λ (t₁,f₁) → (t₁, λt → g (f₁ t))) ≡
// λ (t₁, f₁) → (λ (t₁,f₁) → (t₁, λt → f (f₁ t))) (
//   (λ (t₁,f₁) → (t₁, λt → g (f₁ t))) (t₁, f₁) )  invoke defn. of ∘
// ≡
// λ (t₁, f₁) → (λ (t₁,f₁) → (t₁, λt → f (f₁ t))) (
//   (t₁, λt → g (f₁ t)))
// ≡
// λ (t₁, f₁) → (t₁, λt → f ((λt → g (f₁ t)) t))
// ≡
// λ (t₁, f₁) → (t₁, λt → f (g (f₁ t))
//..
// Now we can conclude the our proof
//..
// μ⟦BehaviorMap()⟧ (f ∘ g) ≡ (μ⟦BehaviorMap()⟧ f) ∘ (μ⟦BehaviorMap()⟧ g)
//..
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Adding behaviors
// - - - - - - - - - - - - - -
// 'BehaviorMap' provides an easy way to make functions over behaviors that
// already have functions over values. Take, for instance, addition of 'int'
// behaviors as an example.
//..
//  sfrp::Behavior<int> addBehaviors(sfrp::Behavior<int> lhs,
//                                   sfrp::Behavior<int> rhs) {
//    return sfrp::BehaviorMap()([](int a, int b) { return a + b; }, lhs, rhs);
//  }
//..
// The function argument to 'BehaviorMap()' need no be an anonymous function.
// It can indeed be a function pointor or a functor with a 'const' 'operator()'
// function.

#include <sfrp/behavior.hpp>
#include <sfrp/mapvaluepullfunc.hpp>

namespace sfrp {

// This class implements a functor that applies a function to the values within
// one or more behaviors, resulting in a new behavior.
struct BehaviorMap {
  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return a behavior that, for any time 't', is defined to be the specified
  // 'function' applied to the specified 'argBehaviors' behaviors at time 't'.
  // If any of 'argBehaviors' is not defined at time 't', then neither is the
  // result behavior.
  //
  // The behavior is undefined unless the specified 'Function' meets the
  // 'Deferred Callable Object' concept of Boost.Fusion.
  template <typename Function, typename... ArgBehaviors>
  typename result<BehaviorMap(Function, ArgBehaviors...)>::type operator()(
      Function function,
      ArgBehaviors... argBehaviors) const;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename... ArgBehaviors>
struct BehaviorMap::result<BehaviorMap(Function, ArgBehaviors...)> {
  typedef typename MapValuePullFunc_Result<Function, ArgBehaviors...>::type
      OptionalBehaviorValue;
  typedef typename OptionalBehaviorValue::value_type BehaviorValue;
  typedef sfrp::Behavior<BehaviorValue> type;
};

template <typename Function, typename... ArgBehaviors>
typename BehaviorMap::result<BehaviorMap(Function, ArgBehaviors...)>::type
BehaviorMap::
operator()(Function function, ArgBehaviors... argBehaviors) const {
  typedef result<BehaviorMap(Function, ArgBehaviors...)>::type Result;
  return Result::fromValuePullFunc(
      MapValuePullFunc<Function, ArgBehaviors...>(function, argBehaviors...));
}
}
#endif

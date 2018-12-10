#pragma once

/*! @file */

#include <limits>

#include <flecsi/execution/execution.h>

namespace flecsi {
namespace execution {
namespace reduction {

#define flecsi_register_operation_types(operation)                           \
  flecsi_register_reduction_operation(operation, int);                         \
  flecsi_register_reduction_operation(operation, long);                        \
  flecsi_register_reduction_operation(operation, short);                       \
  flecsi_register_reduction_operation(operation, unsigned);                    \
  flecsi_register_reduction_operation(operation, size_t);                      \
  flecsi_register_reduction_operation(operation, float);                       \
  flecsi_register_reduction_operation(operation, double);

//----------------------------------------------------------------------------//
// Min
//----------------------------------------------------------------------------//

/*!
  Minimum reduction type.
 */

template<typename T>
struct min {

  using LHS = T;
  using RHS = T;
  static constexpr T identity{};

  template<bool EXCLUSIVE = true>
  static void apply(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs = lhs < rhs ? lhs : rhs;
    }
    else {
    } // if constexpr

  } // apply

  template<bool EXCLUSIVE = true>
  static void fold(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs = lhs < rhs ? lhs : rhs;
    }
    else {
    } // if constexpr

  } // fold

}; // struct min

flecsi_register_operation_types(min);

//----------------------------------------------------------------------------//
// Max
//----------------------------------------------------------------------------//

/*!
  Maximum reduction type.
 */

template<typename T>
struct max {

  using LHS = T;
  using RHS = T;
  static constexpr T identity{};

  template<bool EXCLUSIVE = true>
  static void apply(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs = lhs > rhs ? lhs : rhs;
    }
    else {
    } // if constexpr

  } // apply

  template<bool EXCLUSIVE = true>
  static void fold(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs = lhs > rhs ? lhs : rhs;
    }
    else {
    } // if constexpr

  } // fold

}; // struct max

flecsi_register_operation_types(max);

//----------------------------------------------------------------------------//
// Sum
//----------------------------------------------------------------------------//

/*!
  Sum reduction type.
 */

template<typename T>
struct sum {

  using LHS = T;
  using RHS = T;
  static constexpr T identity{};

  template<bool EXCLUSIVE = true>
  static void apply(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs += rhs;
    }
    else {
    } // if constexpr

  } // apply

  template<bool EXCLUSIVE = true>
  static void fold(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs += rhs;
    }
    else {
    } // if constexpr

  } // fold

}; // struct sum

flecsi_register_operation_types(sum);

/*!
  Product reduction type.
 */

//----------------------------------------------------------------------------//
// Product
//----------------------------------------------------------------------------//

template<typename T>
struct product {

  using LHS = T;
  using RHS = T;
  static constexpr T identity{};

  template<bool EXCLUSIVE = true>
  static void apply(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs *= rhs;
    }
    else {
    } // if constexpr

  } // apply

  template<bool EXCLUSIVE = true>
  static void fold(LHS & lhs, RHS rhs) {

    if constexpr (EXCLUSIVE) {
      lhs *= rhs;
    }
    else {
    } // if constexpr

  } // fold

}; // struct product

flecsi_register_operation_types(product);

} // namespace reduction
} // namespace execution
} // namespace flecsi
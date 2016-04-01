#ifndef __TACHO_CONTROL_HPP__
#define __TACHO_CONTROL_HPP__

/// \file Tacho_Control.hpp
/// \brief A collection of control trees composing high-level variants of algorithms.
/// \author Kyungjoo Kim (kyukim@sandia.gov)

#include "Tacho_Util.hpp"

namespace Tacho {

  // forward declaration for control tree
  template<int ArgAlgo, int ArgVariant>
  struct Control {
    static constexpr int Self[2] = { ArgAlgo, ArgVariant };
  };

  // ----------------------------------------------------------------------------------

  // - DenseByBlocks
  template<> struct Control<AlgoGemm::DenseByBlocks,Variant::One> {
    static constexpr int Gemm[2] = { AlgoGemm::ExternalBlas, Variant::One };
  };

  template<> struct Control<AlgoGemm::DenseByBlocks,Variant::Two> {
    static constexpr int Gemm[2] = { AlgoGemm::InternalBlas, Variant::One };
  };

  template<> struct Control<AlgoTrsm::DenseByBlocks,Variant::One> {
    static constexpr int Gemm[2] = { AlgoGemm::ExternalBlas, Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::ExternalBlas, Variant::One };
  };

  template<> struct Control<AlgoTrsm::DenseByBlocks,Variant::Two> {
    static constexpr int Gemm[2] = { AlgoGemm::InternalBlas, Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::InternalBlas, Variant::One };
  };

  template<> struct Control<AlgoChol::DenseByBlocks,Variant::One> {
    static constexpr int Chol[2] = { AlgoChol::ExternalLapack, Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::ExternalBlas,   Variant::One };
    static constexpr int Herk[2] = { AlgoHerk::ExternalBlas,   Variant::One };
    static constexpr int Gemm[2] = { AlgoGemm::ExternalBlas,   Variant::One };
  };

  template<> struct Control<AlgoChol::DenseByBlocks,Variant::Two> {
    static constexpr int Chol[2] = { AlgoChol::ExternalLapack, Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::InternalBlas,   Variant::One };
    static constexpr int Herk[2] = { AlgoHerk::InternalBlas,   Variant::One };
    static constexpr int Gemm[2] = { AlgoGemm::InternalBlas,   Variant::One };
  };

  // - SparseByBlocks
  template<> struct Control<AlgoChol::ByBlocks,Variant::One> {
    static constexpr int Chol[2] = { AlgoChol::Unblocked,        Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::ForFactorization, Variant::One };
    static constexpr int Herk[2] = { AlgoHerk::ForFactorization, Variant::One };
    static constexpr int Gemm[2] = { AlgoGemm::ForFactorization, Variant::One };
  };

  // - SuperNodalByblocks
  template<> struct Control<AlgoChol::ByBlocks,Variant::Two> {
    static constexpr int Chol[2] = { AlgoChol::SuperNodes,    Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::ForSuperNodes, Variant::One };
    static constexpr int Herk[2] = { AlgoHerk::ForSuperNodes, Variant::One };
    static constexpr int Gemm[2] = { AlgoGemm::ForSuperNodes, Variant::One };
  };

  // - Fine grained SuperNodalByblocks
  template<> struct Control<AlgoChol::ByBlocks,Variant::Three> {
    static constexpr int Chol[2] = { AlgoChol::SuperNodesByBlocks,    Variant::One };
    static constexpr int Trsm[2] = { AlgoTrsm::ForSuperNodesByBlocks, Variant::One };
    static constexpr int Herk[2] = { AlgoHerk::ForSuperNodesByBlocks, Variant::One };
    static constexpr int Gemm[2] = { AlgoGemm::ForSuperNodesByBlocks, Variant::One };
  };

  // // ----------------------------------------------------------------------------------

}

#endif
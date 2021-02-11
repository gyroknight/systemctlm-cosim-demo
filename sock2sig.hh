/**
 * @file sock2sig.hh
 * @author Vincent Zhao
 * @brief Converts data received from a TLM-2.0 socket to individual bit
 * signals, adds a data ready-read interface with adjustable timing.
 *
 *
 */

#if !defined(__SOCK2SIG_H__)
#define __SOCK2SIG_H__

#include <tlm_utils/simple_target_socket.h>

#include <memory>
#include <queue>
#include <systemc>

using namespace sc_core;

template <unsigned int BUSWIDTH = 32>
class Sock2Sig : public sc_core::sc_module {
  SC_HAS_PROCESS(Sock2Sig);

 public:
  Sock2Sig(int readyDelay = 0, sc_module_name moduleName = "sock-2-sig");

  tlm_utils::simple_target_socket<Sock2Sig, BUSWIDTH> inputSock;
  sc_vector<sc_out<bool>> outputSigs;
  sc_out<bool> dataReady;
  sc_in<bool> assertRead;

 private:
  void inputSock_b_transport(tlm::tlm_generic_payload& trans, sc_time& delay);
  void updateOutput();

  std::queue<std::unique_ptr<std::vector<uint8_t>>> buffer;
  std::unique_ptr<std::vector<uint8_t>> currentData;
  uint8_t nextBitMask;
  ssize_t byteOffset;
  sc_event dataAvailable;
  int readyDelay;
};

#include "sock2sig.tt"

#endif  // __SOCK2SIG_H__
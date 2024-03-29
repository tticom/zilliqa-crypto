/*
 * Copyright (C) 2019 Zilliqa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __MULTISIG_H__
#define __MULTISIG_H__

#include <memory>
#include <vector>

#include "Schnorr.h"
//#include "common/Serializable.h"
#include "../common/Serializable.h"

// Commitment is composed of a random secret scalar, a public point and a hash
// of the public point. It is generated by each signer.

/// CommitSecret stores information on the secret scalar.
struct CommitSecret : public Serializable {
  /// The secret value.
  std::shared_ptr<BIGNUM> m_s;

  /// Flag to indicate if parameters have been initialized.
  bool m_initialized;

  /// Constructor for generating a new commitment secret.
  CommitSecret();

  /// Constructor for loading existing secret from a byte stream.
  CommitSecret(const bytes& src, unsigned int offset);

  /// Copy constructor.
  CommitSecret(const CommitSecret& src);

  /// Destructor.
  ~CommitSecret();

  /// Indicates if secret parameters have been initialized.
  bool Initialized() const;

  /// Implements the Serialize function inherited from Serializable.
  unsigned int Serialize(bytes& dst, unsigned int offset) const;

  /// Implements the Deserialize function inherited from Serializable.
  int Deserialize(const bytes& src, unsigned int offset);

  /// Assignment operator.
  CommitSecret& operator=(const CommitSecret&);

  /// Equality comparison operator.
  bool operator==(const CommitSecret& r) const;
};

/// Stores information on the public point.
struct CommitPoint : public Serializable {
  /// The public point.
  std::shared_ptr<EC_POINT> m_p;

  /// Flag to indicate if parameters have been initialized.
  bool m_initialized;

  /// Default constructor for an uninitialized point.
  CommitPoint();

  /// Constructor for generating a new commitment point from specified
  /// CommitSecret.
  CommitPoint(const CommitSecret& secret);

  /// Constructor for loading public point information from a byte stream.
  CommitPoint(const bytes& src, unsigned int offset);

  /// Copy constructor.
  CommitPoint(const CommitPoint&);

  /// Destructor.
  ~CommitPoint();

  /// Indicates if commitment point parameters have been initialized.
  bool Initialized() const;

  /// Implements the Serialize function inherited from Serializable.
  unsigned int Serialize(bytes& dst, unsigned int offset) const;

  /// Implements the Deserialize function inherited from Serializable.
  int Deserialize(const bytes& src, unsigned int offset);

  /// Sets the commitment point value based on the specified CommitSecret.
  void Set(const CommitSecret& secret);

  /// Assignment operator.
  CommitPoint& operator=(const CommitPoint& src);

  /// Equality comparison operator.
  bool operator==(const CommitPoint& r) const;
};

/// Stores information on the hash of the point.
struct CommitPointHash : public Serializable {
  /// The hash value.
  std::shared_ptr<BIGNUM> m_h;

  /// Flag to indicate if parameters have been initialized.
  bool m_initialized;

  /// Default constructor for an uninitialized hash value.
  CommitPointHash();

  /// Constructor for generating a new hash value from a specified point.
  CommitPointHash(const CommitPoint& point);

  /// Constructor for loading existing hash from a byte stream.
  CommitPointHash(const bytes& src, unsigned int offset);

  /// Copy constructor.
  CommitPointHash(const CommitPointHash& src);

  /// Destructor.
  ~CommitPointHash();

  /// Indicates if hash point parameters have been initialized.
  bool Initialized() const;

  /// Implements the Serialize function inherited from Serializable.
  unsigned int Serialize(bytes& dst, unsigned int offset) const;

  /// Implements the Deserialize function inherited from Serializable.
  int Deserialize(const bytes& src, unsigned int offset);

  /// Sets the hash point value based on the specified CommitPoint.
  void Set(const CommitPoint& point);

  /// Assignment operator.
  CommitPointHash& operator=(const CommitPointHash& src);

  /// Equality comparison operator.
  bool operator==(const CommitPointHash& r) const;

  /// Utility std::string conversion function for signature info.
  explicit operator std::string() const {
    std::string temp;
    if (!DataConversion::SerializableToHexStr(*this, temp)) {
      return "";
    }
    return "0x" + temp;
  }
};

/// Stores information on the challenge generated by the aggregator in the
/// EC-Schnorr multisignature scheme.
struct Challenge : public Serializable {
  /// The challenge value.
  std::shared_ptr<BIGNUM> m_c;

  /// Flag to indicate if parameters have been initialized.
  bool m_initialized;

  /// Default constructor for an uninitialized challenge.
  Challenge();

  /// Constructor for generating a new challenge.
  Challenge(const CommitPoint& aggregatedCommit, const PubKey& aggregatedPubkey,
            const bytes& message);

  /// Constructor for generating a new challenge (with size and offset)
  Challenge(const CommitPoint& aggregatedCommit, const PubKey& aggregatedPubkey,
            const bytes& message, unsigned int offset, unsigned int size);

  /// Constructor for loading challenge information from a byte stream.
  Challenge(const bytes& src, unsigned int offset);

  /// Copy constructor.
  Challenge(const Challenge& src);

  /// Destructor.
  ~Challenge();

  /// Indicates if challenge parameters have been initialized.
  bool Initialized() const;

  /// Implements the Serialize function inherited from Serializable.
  unsigned int Serialize(bytes& dst, unsigned int offset) const;

  /// Implements the Deserialize function inherited from Serializable.
  int Deserialize(const bytes& src, unsigned int offset);

  /// Sets the challenge value based on the specified input parameters.
  void Set(const CommitPoint& aggregatedCommit, const PubKey& aggregatedPubkey,
           const bytes& message, unsigned int offset, unsigned int size);

  /// Assignment operator.
  Challenge& operator=(const Challenge& src);

  /// Equality comparison operator.
  bool operator==(const Challenge& r) const;
};

/// Stores information on the response generated by each signer in the
/// EC-Schnorr multisignature scheme.
struct Response : public Serializable {
  /// The response value.
  std::shared_ptr<BIGNUM> m_r;

  /// Flag to indicate if parameters have been initialized.
  bool m_initialized;

  /// Default constructor for an uninitialized response.
  Response();

  /// Constructor for generating a new response.
  Response(const CommitSecret& secret, const Challenge& challenge,
           const PrivKey& privkey);

  /// Constructor for loading response information from a byte stream.
  Response(const bytes& src, unsigned int offset);

  /// Copy constructor.
  Response(const Response& src);

  /// Destructor.
  ~Response();

  /// Indicates if response parameters have been initialized.
  bool Initialized() const;

  /// Implements the Serialize function inherited from Serializable.
  unsigned int Serialize(bytes& dst, unsigned int offset) const;

  /// Implements the Deserialize function inherited from Serializable.
  int Deserialize(const bytes& src, unsigned int offset);

  /// Sets the response value based on the specified input parameters.
  void Set(const CommitSecret& secret, const Challenge& challenge,
           const PrivKey& privkey);

  /// Assignment operator.
  Response& operator=(const Response& src);

  /// Equality comparison operator.
  bool operator==(const Response& r) const;
};

/// Implements the functionality for EC-Schnorr multisignature scheme
/// operations.
class MultiSig {
  MultiSig();
  ~MultiSig();

  MultiSig(MultiSig const&) = delete;
  void operator=(MultiSig const&) = delete;

  std::mutex m_mutexMultiSigVerify;

 public:
  /// Returns a MultiSig instance.
  static MultiSig& GetInstance();

  /// Aggregates the public keys for the multisignature aggregator.
  static std::shared_ptr<PubKey> AggregatePubKeys(
      const std::vector<PubKey>& pubkeys);

  /// Aggregates the received commitments for the multisignature aggregator.
  static std::shared_ptr<CommitPoint> AggregateCommits(
      const std::vector<CommitPoint>& commitPoints);

  /// Aggregates the received responses for the multisignature aggregator.
  static std::shared_ptr<Response> AggregateResponses(
      const std::vector<Response>& responses);

  /// Generates the aggregated signature for the multisignature aggregator.
  static std::shared_ptr<Signature> AggregateSign(
      const Challenge& challenge, const Response& aggregatedResponse);

  /// Verifies a response for the multisignature aggregator.
  static bool VerifyResponse(const Response& response,
                             const Challenge& challenge, const PubKey& pubkey,
                             const CommitPoint& commitPoint);

  /// Checks the multi-signature validity using EC curve parameters and the
  /// specified aggregated PubKey.
  bool MultiSigVerify(const bytes& message, const Signature& toverify,
                      const PubKey& pubkey);

  /// Checks the multi-signature validity using EC curve parameters and the
  /// specified aggregated PubKey.
  bool MultiSigVerify(const bytes& message, unsigned int offset,
                      unsigned int size, const Signature& toverify,
                      const PubKey& pubkey);

  /// Wrapper function for signing PoW message (including public key) for
  /// Proof-of-Possession (PoP) phase
  static bool SignKey(const bytes& messageWithPubKey, const PairOfKey& keyPair,
                      Signature& signature);

  /// Wrapper function for verifying PoW message (including public key) for
  /// Proof-of-Possession (PoP) phase
  static bool VerifyKey(const bytes& messageWithPubKey,
                        const Signature& signature, const PubKey& pubKey);
};

#endif  // __MULTISIG_H__

/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HKS_TYPES_H
#define HKS_TYPES_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HKS_API_PUBLIC
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#ifdef HKS_DLL_EXPORT
#define HKS_DLL_API_PUBLIC __declspec(dllexport)
#else
#define HKS_DLL_API_PUBLIC __declspec(dllimport)
#endif
#else
#define HKS_DLL_API_PUBLIC __attribute__ ((visibility("default")))
#endif
#else
#define HKS_DLL_API_PUBLIC __attribute__ ((visibility("default")))
#endif

#define HKS_SDK_VERSION                          "1.0.0.10"
#define HKS_BOOL_FALSE                           0
#define HKS_BOOL_TRUE                            1
#define HKS_ALIAS_MAX_SIZE                       64
#define HKS_SALT_MAX_SIZE                        16
#define HKS_NONCE_MIN_SIZE                       7
#define HKS_KEY_BYTES_CURVE25519                 32
#define HKS_RANDOM_MAX_LEN                       1024
#define HKS_MAX_KEY_LEN_128                      128
#define HKS_MAX_KEY_LEN_192                      192
#define HKS_MAX_KEY_LEN_256                      256
#define HKS_KEY_DERIVE_LEN                       32
#define HKS_BINARY_OF_DEC                        10
#define HKS_BINARY_OF_HEX                        16
#define HKS_HASH256_MIN_OUT_SIZE                 32
#define HKS_HASH512_MIN_OUT_SIZE                 64
#define HKS_DERIVED_KEY_MIN_OUT_SIZE             16
#define HKS_BITS_PER_BYTES                       8
#define HKS_SIGNATURE_MIN_SIZE                   64
#define HKS_RSA2048_SIGNATURE_SIZE               256
#define HKS_PUBLIC_BYTES_ED25519                 32
#define HKS_PRIVATE_BYTES_ED25519                64
#define HKS_KEY_PAIR_CIPHER_ED25519              80
#define HKS_HEADER_HASH_SIZE                     64
#define HKS_AUTH_ID_MAX_SIZE                     64
#define HKS_KEY_LEN_RSA_KEYPAIR                  2048
#define HKS_CHALLENGE_MIN_LEN                    16
#define HKS_CHALLENGE_MAX_LEN                    128
#define HKS_ENCRYPTED_KEY_TAG_LEN                16
#define HKS_ENCRYPTED_KEY_NONCE_LEN              16
#define HKS_ENCRYPTED_KEY_AAD_LEN                16

/* AES encrypt tag max length */
#define HKS_ENCRYPT_MAX_TAG_SIZE               ((uint8_t)32)

/* Data blob type and related macros */
#define HKS_BLOB_TYPE_RAW                      ((uint8_t)0x00)
#define HKS_BLOB_TYPE_ALIAS                    ((uint8_t)0x01)
#define HKS_BLOB_TYPE_KEY                      ((uint8_t)0x02)
#define HKS_BLOB_TYPE_ENCRYPTED_KEY            ((uint8_t)0x03)
#define HKS_BLOB_TYPE_MESSAGE                  ((uint8_t)0x04)
#define HKS_BLOB_TYPE_HASH                     ((uint8_t)0x05)
#define HKS_BLOB_TYPE_MAC                      ((uint8_t)0x06)
#define HKS_BLOB_TYPE_LABEL                    ((uint8_t)0x07)
#define HKS_BLOB_TYPE_SIGNATURE                ((uint8_t)0x08)
#define HKS_BLOB_TYPE_IV                       ((uint8_t)0x09)
#define HKS_BLOB_TYPE_AAD                      ((uint8_t)0x0a)
#define HKS_BLOB_TYPE_SALT                     ((uint8_t)0x0b)
#define HKS_BLOB_TYPE_PLAIN_TEXT               ((uint8_t)0x0c)
#define HKS_BLOB_TYPE_CIPHER_TEXT              ((uint8_t)0x0d)
#define HKS_BLOB_TYPE_MATERIAL                 ((uint8_t)0x0e)
#define HKS_BLOB_TYPE_AUTH_ID                  ((uint8_t)0x10)
#define HKS_BLOB_TYPE_BUFFER                   ((uint8_t)0x12)

struct hks_blob {
    uint8_t type;
    uint8_t *data;
    uint32_t size;
};

/* HKS_ECC_CURVE_CURVE25519 */
#define HKS_ECC_CURVE_CURVE25519               ((uint16_t)0x001d)

#define HKS_ECC_CURVE_ED25519                  ((uint16_t)0x8001)

#define HKS_KEY_TYPE_RSA_PUBLIC_KEY            ((uint32_t)0x60010000)

#define HKS_KEY_TYPE_RSA_KEYPAIR               ((uint32_t)0x70010000)

#define HKS_KEY_TYPE_ECC_PUBLIC_KEY_BASE       ((uint32_t)0x60030000)

#define HKS_KEY_TYPE_ECC_KEYPAIR_BASE          ((uint32_t)0x70030000)

#define HKS_KEY_TYPE_ECC_CURVE_MASK            ((uint32_t)0x0000ffff)

#define hks_key_type_ecc_public_key(curve) \
    (HKS_KEY_TYPE_ECC_PUBLIC_KEY_BASE | (curve))

#define hks_key_type_ecc_key_pair(curve) \
    (HKS_KEY_TYPE_ECC_KEYPAIR_BASE | (curve))

#define HKS_KEY_TYPE_ECC_PUBLIC_KEY_CURVE25519 \
    (hks_key_type_ecc_public_key(HKS_ECC_CURVE_CURVE25519))

#define HKS_KEY_TYPE_ECC_KEYPAIR_CURVE25519 \
    (hks_key_type_ecc_key_pair(HKS_ECC_CURVE_CURVE25519))

#define HKS_KEY_TYPE_EDDSA_PUBLIC_KEY_BASE     ((uint32_t)0xe0010000)

#define HKS_KEY_TYPE_EDDSA_KEYPAIR_BASE        ((uint32_t)0xf0010000)

#define hks_key_type_eddsa_public_key(curve) \
    (HKS_KEY_TYPE_EDDSA_PUBLIC_KEY_BASE | (curve))

#define hks_key_type_eddsa_key_pair(curve) \
    (HKS_KEY_TYPE_EDDSA_KEYPAIR_BASE | (curve))

#define HKS_KEY_TYPE_EDDSA_PUBLIC_KEY_ED25519 \
    (hks_key_type_eddsa_public_key(HKS_ECC_CURVE_CURVE25519))

#define HKS_KEY_TYPE_EDDSA_KEYPAIR_ED25519 \
    (hks_key_type_eddsa_key_pair(HKS_ECC_CURVE_CURVE25519))


#define HKS_KEY_TYPE_AES                       ((uint32_t)0x40000001)

#define HKS_KEY_TYPE_HMAC                      ((uint32_t)0x51000000)

#define HKS_KEY_TYPE_DERIVE                    ((uint32_t)0x52000000)

/* key usage */
#define HKS_KEY_USAGE_EXPORT                   ((uint32_t)0x00000001)

#define HKS_KEY_USAGE_ENCRYPT                  ((uint32_t)0x00000100)

#define HKS_KEY_USAGE_DECRYPT                  ((uint32_t)0x00000200)

#define HKS_KEY_USAGE_SIGN                     ((uint32_t)0x00000400)

#define HKS_KEY_USAGE_VERIFY                   ((uint32_t)0x00000800)

#define HKS_KEY_USAGE_DERIVE                   ((uint32_t)0x00001000)

#ifdef _SUPPORT_HKS_TEE_
#define HKS_KEY_USAGE_WRAP                     ((uint32_t)0x00010000)
#define HKS_KEY_USAGE_UNWRAP                   ((uint32_t)0x00020000)
#endif
/* algorithm padding */
#define HKS_PADDING_NONE                       ((uint32_t)0x00000000)
#define HKS_PADDING_PKCS7                      ((uint32_t)0x00000001)
#define HKS_PADDING_PSS                        ((uint32_t)0x00000002)
#define HKS_PADDING_OAEP                       ((uint32_t)0x00000003)
#define HKS_PADDING_PKCS1_5                    ((uint32_t)0x00000004)
#define HKS_PADDING_PKCS2_1                    ((uint32_t)0x00000005)

/* mode */
#define HKS_MODE_CBC                           ((uint32_t)0x04600101)

#define HKS_MODE_GCM                           ((uint32_t)0x06001002)

/* hash algorithms */
#define HKS_ALG_DIGEST_NONE                    ((uint32_t)0x00000000)

#define HKS_ALG_HASH_MASK                      ((uint32_t)0x000000ff)

#define HKS_ALG_HASH_SHA_1                     ((uint32_t)0x01000005)

#define HKS_ALG_HASH_SHA_256                   ((uint32_t)0x01000009)

#define HKS_ALG_HASH_SHA_384                   ((uint32_t)0x0100000a)

#define HKS_ALG_HASH_SHA_512                   ((uint32_t)0x0100000b)

/* mac algorithms */
#define HKS_ALG_HMAC_BASE                      ((uint32_t)0x02800000)

#define hks_alg_hmac(hash_alg) \
    (HKS_ALG_HMAC_BASE | ((hash_alg) & HKS_ALG_HASH_MASK))

/* AEAD algorithms */
#define HKS_ALG_CCM                            ((uint32_t)0x06001001)
#define HKS_ALG_GCM                            ((uint32_t)0x06001002)
#define HKS_ALG_CBC                            ((uint32_t)0x06001003)

/* HKDF algorithms */
#define HKS_ALG_HKDF_BASE                      ((uint32_t)0x30000100)
#define hks_alg_hkdf(hash_alg) \
    (HKS_ALG_HKDF_BASE | ((hash_alg) & HKS_ALG_HASH_MASK))

/* Key agreement/derivation algorithm */
#define HKS_ALG_SELECT_RAW                     ((uint32_t)0x31000001)
#define HKS_ALG_ECDH_BASE                      ((uint32_t)0x22200000)
#define HKS_ALG_KEY_DERIVATION_MASK            ((uint32_t)0x010fffff)
#define hks_alg_ecdh(kdf_alg) (HKS_ALG_ECDH_BASE | ((kdf_alg) & HKS_ALG_KEY_DERIVATION_MASK))

struct hks_key_param {
    uint32_t key_type; /* algorithm */
    uint32_t key_len;
    uint32_t key_usage; /* usage */
    uint32_t key_pad; /* Fill mode */
    uint32_t key_mode; /* Group mode */
    uint32_t key_role; /* role */
    uint16_t key_domain;
    struct hks_blob key_auth_id; /* auth id */
};

struct hks_crypt_param {
    struct hks_blob nonce; /* Nonce value or iv vector */
    struct hks_blob aad;
};

/*
 * log interface
 * tag - module name, default "HKS"
 */
typedef void(*hks_log_func)(const char *tag, const char *func_name,
    const char *format, ...);

struct hks_log_f_group {
    hks_log_func log_info;
    hks_log_func log_warn;
    hks_log_func log_error;
    hks_log_func log_debug;
};

struct hks_encrypt_material {
    struct hks_blob *cipher_text;
    struct hks_blob *nonce_blob; /* Nonce value or iv vector */
    struct hks_blob *aad_blob;
    struct hks_blob *plain_text;
    struct hks_storage_key_info *key_info;
    uint32_t sealing_alg;
};

enum hks_pki_cmd_type {
    CMD_PKI_PROVISION = 1,
    CMD_PKI_VERIFY,
    CMD_EFUSE_ROOT_SALT_WRITE,
    CMD_EFUSE_ROOT_SALT_VERIFY,
};

#define HKS_MAX_CERT_NUM                  3
#define HKS_MIN_SIGNATURE_SIZE            256
#define HKS_GET_CAPABILITES_SIZE          512

struct hks_cert_chain {
    uint32_t count;
    struct hks_blob *cert; /* cert array list */
};

struct hks_usage_spec {
    uint32_t mode;
    uint32_t padding;
    uint32_t digest;
    void *param; /* extended param */
};

#ifdef __cplusplus
}
#endif

#endif /* HKS_TYPES_H */

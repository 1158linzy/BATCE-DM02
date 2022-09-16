/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "securec.h"

#include "app_rsa.h"
#include "hota_updater.h"
#include "hota_verify.h"

using namespace std;
using namespace testing::ext;

static const int READ_BUF_LEN = 256;
static char g_readBuf[READ_BUF_LEN];

static const int OTA_PKG_SIZE = 16 * 48 + 3;
static const char *PASS_THROUGH_PATH = "/sdcard/update/ota_pkg_pass_through.bin";
static const int INFO_COMP_DATA_LEN = 267;
static const int WRONG_PKG_TYPE = 128;

/* this pkg is packed by same pubkey */
static char g_otaPkg[OTA_PKG_SIZE] = {
    0x01, 0x00, 0x88, 0x00, 0x0B, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x48, 0x61, 0x72, 0x6D,
    0x6F, 0x6E, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x70, 0x65, 0x6E,
    0x48, 0x61, 0x72, 0x6D, 0x6F, 0x6E, 0x79, 0x20, 0x31, 0x2E, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00,
    0x32, 0x30, 0x32, 0x30, 0x2E, 0x31, 0x31, 0x2E, 0x30, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x31, 0x36, 0x3A, 0x31, 0x36, 0x3A, 0x33, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x47, 0x00, 0x63, 0x6F, 0x6E, 0x66, 0x69, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x05, 0x00, 0x00, 0x31, 0x2E, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x10, 0xA3, 0x5B, 0xE1,
    0xA4, 0x09, 0x71, 0x81, 0xFE, 0x4C, 0x19, 0xDF, 0xAB, 0x90, 0x30, 0x7B, 0x86, 0x15, 0x30, 0xAA,
    0xCA, 0x4E, 0x21, 0x76, 0x8C, 0x55, 0x55, 0x82, 0xC0, 0x4E, 0x2A, 0x75, 0x70, 0x64, 0x61, 0x74,
    0x65, 0x2F, 0x69, 0x6E, 0x66, 0x6F, 0x2E, 0x62, 0x69, 0x6E, 0x00, 0x1A, 0x0C, 0xAC, 0x62, 0x06,
    0x38, 0xA9, 0x06, 0x72, 0xBD, 0x89, 0xA8, 0xB5, 0x1E, 0x28, 0xF3, 0x89, 0x9A, 0xEF, 0xAA, 0x9C,
    0x7E, 0xB6, 0xF7, 0x55, 0xA1, 0x8D, 0x3F, 0x66, 0x68, 0xC2, 0x5D, 0x87, 0x34, 0xA1, 0x70, 0x7A,
    0xAA, 0x88, 0xBE, 0xAF, 0x49, 0x44, 0xAB, 0x1B, 0xFA, 0x7A, 0x89, 0x3E, 0x15, 0x11, 0x6F, 0x6F,
    0xF4, 0x75, 0x8F, 0x2C, 0xF0, 0x77, 0x5A, 0x54, 0xB1, 0x95, 0xE0, 0xDB, 0x6C, 0x31, 0x1C, 0xF3,
    0x4D, 0x50, 0xA8, 0x64, 0x61, 0x42, 0xA7, 0x81, 0x76, 0x32, 0x39, 0xC7, 0x16, 0xA2, 0x6A, 0xC4,
    0x2F, 0x9E, 0x5D, 0xD3, 0x7D, 0x80, 0x3C, 0x75, 0x18, 0x5A, 0xB2, 0xB6, 0x56, 0xAE, 0xA9, 0xE4,
    0x07, 0x9A, 0x73, 0xDC, 0x4A, 0x29, 0x20, 0x09, 0x60, 0x7D, 0x98, 0x1D, 0x52, 0x6E, 0x5D, 0x13,
    0x10, 0xAD, 0xC9, 0xE8, 0x8F, 0xC7, 0x43, 0xC5, 0x35, 0x02, 0xE4, 0x4A, 0x15, 0x85, 0xA4, 0x0C,
    0x30, 0x37, 0x54, 0x66, 0x2D, 0xEE, 0x08, 0x96, 0x1E, 0x47, 0xDF, 0x1E, 0x8E, 0xC4, 0x20, 0x65,
    0xE9, 0xD7, 0x08, 0x9F, 0x78, 0xF5, 0xB6, 0x36, 0x32, 0xC2, 0x0E, 0xBB, 0x9D, 0x02, 0x8A, 0xBE,
    0x2B, 0xE4, 0x25, 0x72, 0x06, 0x0B, 0x13, 0x6C, 0x27, 0x4E, 0xFA, 0x39, 0x48, 0x95, 0x80, 0x19,
    0x69, 0x19, 0xCE, 0xDC, 0xCF, 0x8F, 0xAC, 0x1E, 0xCF, 0x23, 0x15, 0x27, 0x0D, 0xF1, 0xE2, 0xFD,
    0x8F, 0x46, 0x43, 0x3B, 0x7D, 0xCF, 0x7A, 0x60, 0x5A, 0x4E, 0x41, 0xB9, 0x46, 0xF6, 0x2E, 0x45,
    0xBB, 0xC5, 0x5F, 0x9E, 0x2E, 0xA7, 0x08, 0x89, 0xEF, 0xB6, 0xDD, 0x39, 0x55, 0x3B, 0xD8, 0xC6,
    0xEF, 0xE9, 0xFF, 0x0C, 0xE2, 0x3B, 0xCB, 0xB0, 0x71, 0x16, 0xD7, 0xFF, 0x5A, 0x7C, 0xBF, 0xF9,
    0xD5, 0xB1, 0x89, 0x2E, 0x09, 0x8D, 0xC1, 0x37, 0x94, 0xF7, 0xD2, 0x73, 0x65, 0x74, 0x65, 0x6E,
    0x76, 0x20, 0x62, 0x6F, 0x6F, 0x74, 0x61, 0x72, 0x67, 0x73, 0x20, 0x27, 0x6D, 0x65, 0x6D, 0x3D,
    0x35, 0x31, 0x32, 0x4D, 0x20, 0x63, 0x6F, 0x6E, 0x73, 0x6F, 0x6C, 0x65, 0x3D, 0x74, 0x74, 0x79,
    0x41, 0x4D, 0x41, 0x30, 0x2C, 0x31, 0x31, 0x35, 0x32, 0x30, 0x30, 0x20, 0x63, 0x6C, 0x6B, 0x5F,
    0x69, 0x67, 0x6E, 0x6F, 0x72, 0x65, 0x5F, 0x75, 0x6E, 0x75, 0x73, 0x65, 0x64, 0x20, 0x72, 0x77,
    0x20, 0x72, 0x6F, 0x6F, 0x74, 0x3D, 0x2F, 0x64, 0x65, 0x76, 0x2F, 0x6D, 0x74, 0x64, 0x62, 0x6C,
    0x6F, 0x63, 0x6B, 0x32, 0x20, 0x72, 0x6F, 0x6F, 0x74, 0x66, 0x73, 0x74, 0x79, 0x70, 0x65, 0x3D,
    0x6A, 0x66, 0x66, 0x73, 0x32, 0x20, 0x6D, 0x74, 0x64, 0x70, 0x61, 0x72, 0x74, 0x73, 0x3D, 0x68,
    0x69, 0x5F, 0x73, 0x66, 0x63, 0x3A, 0x31, 0x4D, 0x28, 0x75, 0x62, 0x6F, 0x6F, 0x74, 0x2E, 0x62,
    0x69, 0x6E, 0x29, 0x2C, 0x39, 0x4D, 0x28, 0x6C, 0x69, 0x74, 0x65, 0x6F, 0x73, 0x2E, 0x62, 0x69,
    0x6E, 0x29, 0x2C, 0x31, 0x36, 0x4D, 0x28, 0x72, 0x6F, 0x6F, 0x74, 0x66, 0x73, 0x2E, 0x6A, 0x66,
    0x66, 0x73, 0x32, 0x29, 0x20, 0x27, 0x20, 0x73, 0x65, 0x74, 0x65, 0x6E, 0x76, 0x20, 0x62, 0x6F,
    0x6F, 0x74, 0x63, 0x6D, 0x64, 0x20, 0x27, 0x73, 0x66, 0x20, 0x70, 0x72, 0x6F, 0x62, 0x65, 0x20,
    0x30, 0x3B, 0x73, 0x66, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x30, 0x78, 0x34, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x20, 0x30, 0x78, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x30, 0x78,
    0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3B, 0x67, 0x6F, 0x20, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x27
};

/* this pkg is packed by different pubkey */
static char g_otaPkgWrongKey[OTA_PKG_SIZE] = {
    0x01, 0x00, 0x88, 0x00, 0x0B, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x48, 0x61, 0x72, 0x6D,
    0x6F, 0x6E, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x70, 0x65, 0x6E,
    0x48, 0x61, 0x72, 0x6D, 0x6F, 0x6E, 0x79, 0x20, 0x31, 0x2E, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00,
    0x32, 0x30, 0x32, 0x30, 0x2E, 0x31, 0x31, 0x2E, 0x30, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x31, 0x30, 0x3A, 0x32, 0x38, 0x3A, 0x34, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x47, 0x00, 0x63, 0x6F, 0x6E, 0x66, 0x69, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x05, 0x00, 0x00, 0x31, 0x2E, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x10, 0xA3, 0x5B, 0xE1,
    0xA4, 0x09, 0x71, 0x81, 0xFE, 0x4C, 0x19, 0xDF, 0xAB, 0x90, 0x30, 0x7B, 0x86, 0x15, 0x30, 0xAA,
    0xCA, 0x4E, 0x21, 0x76, 0x8C, 0x55, 0x55, 0x82, 0xC0, 0x4E, 0x2A, 0x75, 0x70, 0x64, 0x61, 0x74,
    0x65, 0x2F, 0x69, 0x6E, 0x66, 0x6F, 0x2E, 0x62, 0x69, 0x6E, 0x00, 0x5A, 0x53, 0xCE, 0x68, 0xEE,
    0x21, 0xAD, 0x1C, 0xA9, 0x0A, 0x94, 0x50, 0x68, 0x79, 0x3C, 0x6C, 0x60, 0xBC, 0x7B, 0x0F, 0x2E,
    0x46, 0xE5, 0x3B, 0x64, 0x45, 0xEE, 0x26, 0x65, 0x76, 0x2D, 0xE3, 0x4A, 0x09, 0x98, 0x82, 0x2E,
    0x7D, 0xB6, 0x66, 0x13, 0x8C, 0xE7, 0xE2, 0xA9, 0xB5, 0x52, 0xB1, 0x8D, 0x97, 0x99, 0x29, 0xBD,
    0x2E, 0xC8, 0xC7, 0x22, 0x1B, 0x09, 0x03, 0x55, 0x25, 0xA5, 0x9F, 0x39, 0xB6, 0xB5, 0xEE, 0xBD,
    0x16, 0x0E, 0x99, 0xED, 0x7C, 0x4E, 0x14, 0x5A, 0x20, 0xEB, 0xD0, 0x1B, 0x95, 0x11, 0x31, 0x0E,
    0xB7, 0xCB, 0xFB, 0x4F, 0x1A, 0xD9, 0x62, 0x43, 0x9C, 0xB5, 0x32, 0x2B, 0x7D, 0xA1, 0x43, 0x03,
    0x64, 0xEA, 0x02, 0xE4, 0x8F, 0xE3, 0x94, 0x6D, 0x71, 0xBF, 0x3D, 0x8E, 0x0D, 0x40, 0x39, 0x21,
    0xA9, 0xF6, 0xC2, 0xCD, 0x68, 0x4B, 0x1D, 0x8D, 0x60, 0x08, 0xAC, 0xCF, 0x90, 0x12, 0x55, 0x1B,
    0xF0, 0xE1, 0x03, 0xFA, 0xCC, 0xC2, 0xE7, 0x1E, 0x72, 0xD2, 0x91, 0xC1, 0xB4, 0x68, 0x6E, 0x7C,
    0x91, 0x00, 0x6A, 0xCD, 0x58, 0x33, 0x77, 0xBB, 0xE0, 0xBE, 0xD7, 0xFC, 0x1C, 0xDE, 0xA9, 0x2E,
    0x3D, 0x51, 0x07, 0xDB, 0x74, 0xD2, 0x67, 0xA7, 0xDE, 0x06, 0xC7, 0x40, 0x5B, 0x29, 0x42, 0x1F,
    0x76, 0x58, 0x00, 0x4E, 0xFC, 0x45, 0xBF, 0xA1, 0xBC, 0xF8, 0x5D, 0x67, 0x07, 0xA3, 0x89, 0x71,
    0x0D, 0x36, 0xA1, 0xA4, 0x6E, 0xDB, 0xAB, 0x61, 0xE0, 0xD7, 0x20, 0x62, 0x4D, 0x40, 0xF5, 0xAF,
    0x7C, 0x59, 0xF1, 0x4E, 0xD1, 0x06, 0x8F, 0x28, 0xA2, 0x49, 0x7C, 0x47, 0x0A, 0x06, 0x5A, 0xA0,
    0x8F, 0x97, 0x19, 0x3A, 0x86, 0xEC, 0x5E, 0xF7, 0x8E, 0x0D, 0xC8, 0x4C, 0xBB, 0x96, 0x2F, 0x4D,
    0x46, 0x1A, 0x07, 0x76, 0x94, 0x3E, 0x5B, 0x84, 0x5C, 0x26, 0x3B, 0x73, 0x65, 0x74, 0x65, 0x6E,
    0x76, 0x20, 0x62, 0x6F, 0x6F, 0x74, 0x61, 0x72, 0x67, 0x73, 0x20, 0x27, 0x6D, 0x65, 0x6D, 0x3D,
    0x35, 0x31, 0x32, 0x4D, 0x20, 0x63, 0x6F, 0x6E, 0x73, 0x6F, 0x6C, 0x65, 0x3D, 0x74, 0x74, 0x79,
    0x41, 0x4D, 0x41, 0x30, 0x2C, 0x31, 0x31, 0x35, 0x32, 0x30, 0x30, 0x20, 0x63, 0x6C, 0x6B, 0x5F,
    0x69, 0x67, 0x6E, 0x6F, 0x72, 0x65, 0x5F, 0x75, 0x6E, 0x75, 0x73, 0x65, 0x64, 0x20, 0x72, 0x77,
    0x20, 0x72, 0x6F, 0x6F, 0x74, 0x3D, 0x2F, 0x64, 0x65, 0x76, 0x2F, 0x6D, 0x74, 0x64, 0x62, 0x6C,
    0x6F, 0x63, 0x6B, 0x32, 0x20, 0x72, 0x6F, 0x6F, 0x74, 0x66, 0x73, 0x74, 0x79, 0x70, 0x65, 0x3D,
    0x6A, 0x66, 0x66, 0x73, 0x32, 0x20, 0x6D, 0x74, 0x64, 0x70, 0x61, 0x72, 0x74, 0x73, 0x3D, 0x68,
    0x69, 0x5F, 0x73, 0x66, 0x63, 0x3A, 0x31, 0x4D, 0x28, 0x75, 0x62, 0x6F, 0x6F, 0x74, 0x2E, 0x62,
    0x69, 0x6E, 0x29, 0x2C, 0x39, 0x4D, 0x28, 0x6C, 0x69, 0x74, 0x65, 0x6F, 0x73, 0x2E, 0x62, 0x69,
    0x6E, 0x29, 0x2C, 0x31, 0x36, 0x4D, 0x28, 0x72, 0x6F, 0x6F, 0x74, 0x66, 0x73, 0x2E, 0x6A, 0x66,
    0x66, 0x73, 0x32, 0x29, 0x20, 0x27, 0x20, 0x73, 0x65, 0x74, 0x65, 0x6E, 0x76, 0x20, 0x62, 0x6F,
    0x6F, 0x74, 0x63, 0x6D, 0x64, 0x20, 0x27, 0x73, 0x66, 0x20, 0x70, 0x72, 0x6F, 0x62, 0x65, 0x20,
    0x30, 0x3B, 0x73, 0x66, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x30, 0x78, 0x34, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x20, 0x30, 0x78, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x30, 0x78,
    0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3B, 0x67, 0x6F, 0x20, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x27
};

static uint8 g_otaPkgHash[HASH_LENGTH] = {
    0X70, 0X65, 0X9E, 0X30, 0X5A, 0XCF, 0XAC, 0X46, 0X8C, 0X7D, 0X9E, 0XDE, 0X2F, 0XDD, 0XB2, 0X4C,
    0X59, 0XF8, 0XD3, 0X0A, 0XCC, 0X62, 0XEA, 0X39, 0X1A, 0XB3, 0X16, 0X57, 0X6F, 0XBA, 0XDE, 0X74
};

static const char *HotaGetOtaPkg(bool isValid)
{
    return isValid ? g_otaPkg : g_otaPkgWrongKey;
}

static int HotaInterfaceTest()
{
    (void)HotaSetPackageType(USE_DEFAULT_PKG);
    (void)HotaGetUpdateIndex(NULL);
    (void)HotaInit(NULL, NULL);
    (void)HotaWrite(NULL, 0, 0);
    (void)HotaRead(0, 0, NULL);
    (void)HotaCancel();
    return 0;
}

static int HotaWriteTest(const char *otaPkg, int len, int pkgOffset)
{
    int leftLen = len;
    int offset = pkgOffset;
    while (leftLen > 0) {
        int tmpLen = ((leftLen >= READ_BUF_LEN) ? READ_BUF_LEN : leftLen);
        (void)memset_s(g_readBuf, READ_BUF_LEN, 0, READ_BUF_LEN);
        if (memcpy_s(g_readBuf, READ_BUF_LEN, otaPkg + offset, tmpLen) != EOK) {
            printf("memcpy fail!\r\n");
            (void)HotaCancel();
            return -1;
        }
        if (HotaWrite((unsigned char *)g_readBuf, offset, tmpLen) != 0) {
            printf("ota write fail!\r\n");
            (void)HotaCancel();
            return -1;
        }
        offset += READ_BUF_LEN;
        leftLen -= tmpLen;
    }
    return 0;
}

/* isValid: true: use valid pkg, false: use invalid pkg */
static int HotaByDefaultPkg(bool isValid)
{
    if (HotaInit(NULL, NULL) < 0) {
        printf("ota update init fail!\r\n");
        return -1;
    }
    HotaSetPackageType(USE_DEFAULT_PKG);
    int offset = 0;
    int pkgLen = OTA_PKG_SIZE;
    int leftLen = pkgLen;
    const char *otaPkg = HotaGetOtaPkg(isValid);
    if (HotaWriteTest(otaPkg, leftLen, offset) == -1) {
        return -1;
    }
    return 0;
}

static int HotaByDefaultPkgWithWrongHash()
{
    g_otaPkg[OTA_PKG_SIZE - 1] += 1;
    int ret = HotaByDefaultPkg(true);
    g_otaPkg[OTA_PKG_SIZE - 1] -= 1;
    return ret;
}

static int HotaByDefaultPkgWithWrongKey()
{
    return HotaByDefaultPkg(false);
}

static int HotaReadTest(const char *otaPkg, int len, int pkgOffset)
{
    int leftLen = len;
    int offset = pkgOffset;
    while (leftLen > 0) {
        int tmpLen = ((leftLen >= READ_BUF_LEN) ? READ_BUF_LEN : leftLen);
        (void)memset_s(g_readBuf, READ_BUF_LEN, 0, READ_BUF_LEN);
        if (HotaRead(offset, READ_BUF_LEN, (unsigned char *)g_readBuf) != 0) {
            printf("ota write fail!\r\n");
            (void)HotaCancel();
            return -1;
        }
        if (memcmp(g_readBuf, otaPkg + offset, tmpLen) != 0) {
            printf("file content cmp fail!\r\n");
            (void)HotaCancel();
            return -1;
        }
        offset += READ_BUF_LEN;
        leftLen -= tmpLen;
    }

    return 0;
}

static int HotaByNotDefaultPkg()
{
    if (HotaInit(NULL, NULL) < 0) {
        printf("ota update init fail!\r\n");
        return -1;
    }
    HotaSetPackageType(NOT_USE_DEFAULT_PKG);
    int offset = 0;
    int pkgLen = OTA_PKG_SIZE;
    int leftLen = pkgLen;
    const char *otaPkg = HotaGetOtaPkg(true);
    if (HotaWriteTest(otaPkg, leftLen, offset) == -1) {
        return -1;
    }

    int fd = open(PASS_THROUGH_PATH, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("file open failed, fd = %d\r\n", fd);
        (void)HotaCancel();
        return -1;
    }
    int newLen = lseek(fd, 0, SEEK_END);
    if (pkgLen != newLen) {
        printf("pkg len wrong! old = %d, new = %d\r\n", pkgLen, newLen);
        (void)HotaCancel();
        close(fd);
        return -1;
    }
    close(fd);
    leftLen = pkgLen;
    offset = 0;
    if (HotaReadTest(otaPkg, leftLen, offset) == -1) {
        return -1;
    }
    return 0;
}

class OtaTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void OtaTest::SetUpTestCase(void)
{
}

void OtaTest::TearDownTestCase(void)
{
}

void OtaTest::SetUp(void)
{
}

void OtaTest::TearDown(void)
{
    (void)HotaCancel();
}


HWTEST_F(OtaTest, interface_call_001, TestSize.Level4)
{
    EXPECT_EQ(0, HotaInterfaceTest());
}


HWTEST_F(OtaTest, default_pkg_001, TestSize.Level1)
{
    EXPECT_EQ(0, HotaByDefaultPkg(true));
}


HWTEST_F(OtaTest, not_default_pkg_001, TestSize.Level2)
{
    EXPECT_EQ(0, HotaByNotDefaultPkg());
}


HWTEST_F(OtaTest, default_pkg_wrong_hash_001, TestSize.Level3)
{
    EXPECT_EQ(-1, HotaByDefaultPkgWithWrongHash());
}


HWTEST_F(OtaTest, default_pkg_wrong_key_001, TestSize.Level3)
{
    EXPECT_EQ(-1, HotaByDefaultPkgWithWrongKey());
}


HWTEST_F(OtaTest, set_wrong_type_001, TestSize.Level3)
{
    EXPECT_EQ(-1, HotaSetPackageType(WRONG_PKG_TYPE));
}


HWTEST_F(OtaTest, hota_sign_verify_001, TestSize.Level3)
{
    EXPECT_EQ(0, HotaSignVerify(reinterpret_cast<uint8 *>(g_otaPkg), INFO_COMP_DATA_LEN,
        reinterpret_cast<uint8 *>(g_otaPkg + INFO_COMP_DATA_LEN), SIGN_DATA_LEN));
}


HWTEST_F(OtaTest, hota_get_hash_001, TestSize.Level3)
{
    unsigned char hashOut[HASH_LENGTH] = {0};
    EXPECT_EQ(0, HotaGetHash(hashOut, HASH_LENGTH));
}


HWTEST_F(OtaTest, hota_get_key_001, TestSize.Level3)
{
    uint32 length = 0;
    uint8 *keyBuf = HotaGetPubKey(&length);
    EXPECT_NE(0, length);
    EXPECT_NE(true, keyBuf == nullptr);
}


HWTEST_F(OtaTest, hota_decode_key_001, TestSize.Level3)
{
    AppRsaContext rsa;
    uint32 length = 0;

    AppRsaInit(&rsa);
    uint8 *keyBuf = HotaGetPubKey(&length);
    EXPECT_NE(0, length);
    EXPECT_NE(true, keyBuf == nullptr);

    int ret = AppRsaDecodePublicKey(&rsa, keyBuf, length);
    AppRsaFree(&rsa);
    EXPECT_EQ(0, ret);
}


HWTEST_F(OtaTest, hota_verify_data_001, TestSize.Level3)
{
    uint32 length = 0;
    AppRsaContext rsa;

    AppRsaInit(&rsa);
    uint8 *keyBuf = HotaGetPubKey(&length);
    EXPECT_NE(true, keyBuf == nullptr);
    EXPECT_NE(0, length);

    int ret = AppRsaDecodePublicKey(&rsa, keyBuf, length);
    EXPECT_EQ(0, ret);
    ret = AppVerifyData(&rsa, g_otaPkgHash, HASH_LENGTH, reinterpret_cast<uint8 *>(g_otaPkg + INFO_COMP_DATA_LEN),
        SIGN_DATA_LEN);
    AppRsaFree(&rsa);
    EXPECT_EQ(0, ret);
}
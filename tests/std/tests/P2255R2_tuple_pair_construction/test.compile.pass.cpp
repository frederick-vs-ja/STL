// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// TRANSITION, MSVC and EDG haven't implemented intrinsics needed for P2255R2.
#if defined(__clang__) && !defined(__EDG__)
#endif // ^^^ no workaround ^^^

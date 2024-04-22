/*
Copyright (c) 2024, Robert Greener

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include "check_summary.h"
#include <check.h>
#include "contagent/types.h"
#include "contagent/util.h"
#include "contagent/summary.h"

START_TEST(test_sum_activation) {
    belief *bs = malloc(sizeof(belief) * 4);
    GHashTable *acc = g_hash_table_new_full(NULL, NULL, NULL, free_double);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double v = j;
            summary_sum_activation(&bs[j], &v, acc);
        }
    }

    for (int i = 0; i < 4; ++i) {
        // ck_assert_double_eq(*(double *) vs->pdata[i], 4*i);
        ck_assert_double_eq(*(double *)g_hash_table_lookup(acc, &bs[i]), 4*i);
    }
    g_hash_table_unref(acc);
    free(bs);
} END_TEST

Suite *summary_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Summary");
    tc_core = tcase_create("Core");
    
    tcase_add_test(tc_core, test_sum_activation);
    suite_add_tcase(s, tc_core);

    return s;
}

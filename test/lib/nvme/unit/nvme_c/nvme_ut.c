/*-
 *   BSD LICENSE
 *
 *   Copyright (c) Intel Corporation.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "spdk_cunit.h"

#include "nvme/nvme.c"

uint64_t nvme_vtophys(void *buf)
{
	return (uintptr_t)buf;
}

int
nvme_ctrlr_construct(struct spdk_nvme_ctrlr *ctrlr, void *devhandle)
{
	return 0;
}

void
nvme_ctrlr_destruct(struct spdk_nvme_ctrlr *ctrlr)
{
}

int
nvme_ctrlr_process_init(struct spdk_nvme_ctrlr *ctrlr)
{
	return 0;
}

int
nvme_ctrlr_start(struct spdk_nvme_ctrlr *ctrlr)
{
	return 0;
}

void
spdk_nvme_ctrlr_opts_set_defaults(struct spdk_nvme_ctrlr_opts *opts)
{
	memset(opts, 0, sizeof(*opts));
}

static void
test_opc_data_transfer(void)
{
	enum spdk_nvme_data_transfer xfer;

	xfer = spdk_nvme_opc_get_data_transfer(SPDK_NVME_OPC_FLUSH);
	CU_ASSERT(xfer == SPDK_NVME_DATA_NONE);

	xfer = spdk_nvme_opc_get_data_transfer(SPDK_NVME_OPC_WRITE);
	CU_ASSERT(xfer == SPDK_NVME_DATA_HOST_TO_CONTROLLER);

	xfer = spdk_nvme_opc_get_data_transfer(SPDK_NVME_OPC_READ);
	CU_ASSERT(xfer == SPDK_NVME_DATA_CONTROLLER_TO_HOST);

	xfer = spdk_nvme_opc_get_data_transfer(SPDK_NVME_OPC_GET_LOG_PAGE);
	CU_ASSERT(xfer == SPDK_NVME_DATA_CONTROLLER_TO_HOST);
}

int main(int argc, char **argv)
{
	CU_pSuite	suite = NULL;
	unsigned int	num_failures;

	if (CU_initialize_registry() != CUE_SUCCESS) {
		return CU_get_error();
	}

	suite = CU_add_suite("nvme", NULL, NULL);
	if (suite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (
		CU_add_test(suite, "test_opc_data_transfer", test_opc_data_transfer) == NULL
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	num_failures = CU_get_number_of_failures();
	CU_cleanup_registry();
	return num_failures;
}

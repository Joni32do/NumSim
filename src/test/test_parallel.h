#pragma once

#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <array>
#include <vector>

#include "debugging/debug_printer.h"
#include "parallel/partitioning.h"
#include "parallel/communicator.h"
#include "test/test_parallel.h"
#include "discretization/donor_cell.h"


class TestParallel {
    public:
        TestParallel();

        void testCommunicationVector();


        void testRowAndColumn();

};

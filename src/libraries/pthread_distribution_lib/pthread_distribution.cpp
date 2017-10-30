/*
 * Copyright (c) 2017 FH Dortmund and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Description:
 *    pThread Core Affinity Pinning
 *
 * Authors:
 *    M. Ozcelikors,            R.Hottger
 *    <mozcelikors@gmail.com>   <robert.hoettger@fh-dortmund.de>
 *
 * Contributors:
 *
 * Update History:
 *    02.02.2017   -    first compilation
 *    15.03.2017   -    updated tasks for web-based driving
 *
 */


#include <libraries/pthread_distribution_lib/pthread_distribution.h>


int placeThisThreadToCore(int core_id)
{
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id < 0 || core_id >= num_cores)
      return EINVAL;

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);

   pthread_t current_thread = pthread_self();
   int rc = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
   if (rc != 0) {
         std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
   }
   return rc;
}


int placeAThreadToCore (pthread_t thread, int core_id)
{
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id < 0 || core_id >= num_cores)
      return EINVAL;

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);

   int rc = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
   if (rc != 0) {
         std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
   }
   return rc;
}


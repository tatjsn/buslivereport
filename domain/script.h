#pragma once

#include "repository.h"
#include "service.h"

namespace domain {
  class Script {
    private:
      Repository &repository;
      Service &service;
      void run_aggregate_single();

    public:
      Script(Repository &repository, Service &service) :
        repository(repository),
        service(service) {}
      void run_aggregate();
      void run_livereport();
  };
}
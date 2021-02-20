#pragma once

/// @file testsuite/testsuite_support.hpp
/// @brief @copybrief components::TestsuiteSupport

#include <unordered_map>
#include <vector>

#include <cache/cache_update_trait.hpp>
#include <cache/update_type.hpp>
#include <components/component_config.hpp>
#include <components/component_context.hpp>
#include <components/statistics_storage.hpp>
#include <engine/mutex.hpp>
#include <testsuite/cache_control.hpp>
#include <testsuite/component_control.hpp>
#include <testsuite/periodic_task_control.hpp>
#include <testsuite/postgres_control.hpp>
#include <testsuite/redis_control.hpp>
#include <utils/periodic_task.hpp>

/// Testsuite integration
namespace testsuite {}

namespace components {

// clang-format off

/// @ingroup userver_components
///
/// @brief Testsuite support component
///
/// Provides additional functionality for testing, e.g. forced cache updates.
///
/// ## Available options:
/// Name | Description | Default value
/// ---- | ----------- | -------------
/// testsuite-periodic-update-enabled | whether caches update periodically | true
/// testsuite-pg-execute-timeout | execute timeout override for postgres | -
/// testsuite-pg-statement-timeout | statement timeout override for postgres | -
/// testsuite-pg-readonly-master-expected | mutes readonly master detection warning | false
/// testsuite-redis-timeout-connect | minimum connection timeout for redis | -
/// testsuite-redis-timeout-single | minimum single shard timeout for redis | -
/// testsuite-redis-timeout-all | minimum command timeout for redis | -
///
/// ## Configuration example:
///
/// @snippet components/common_component_list_test.cpp  Sample testsuite support component config

// clang-format on

class TestsuiteSupport final : public components::impl::ComponentBase {
 public:
  static constexpr const char* kName = "testsuite-support";

  TestsuiteSupport(const components::ComponentConfig& component_config,
                   const components::ComponentContext& component_context);

  testsuite::CacheControl& GetCacheControl();
  testsuite::ComponentControl& GetComponentControl();
  testsuite::PeriodicTaskControl& GetPeriodicTaskControl();
  const testsuite::PostgresControl& GetPostgresControl();
  const testsuite::RedisControl& GetRedisControl();

  void InvalidateEverything(cache::UpdateType update_type);

  void InvalidateCaches(cache::UpdateType update_type,
                        const std::vector<std::string>& names);

  void ResetMetrics();

 private:
  engine::Mutex invalidation_mutex_;

  testsuite::CacheControl cache_control_;
  testsuite::ComponentControl component_control_;
  testsuite::PeriodicTaskControl periodic_task_control_;
  testsuite::PostgresControl postgres_control_;
  testsuite::RedisControl redis_control_;
  utils::statistics::MetricsStoragePtr metrics_storage_;
};

}  // namespace components

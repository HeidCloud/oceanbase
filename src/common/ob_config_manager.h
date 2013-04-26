/**
 * (C) 2010-2012 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Time-stamp: <2013-04-07 14:16:38 fufeng.syd>
 * Version: $Id$
 * Filename: ob_config_manager.h
 *
 * Authors:
 *   Yudi Shi <fufeng.syd@alipay.com>
 *
 */

#ifndef _OB_CONFIG_MANAGER_H_
#define _OB_CONFIG_MANAGER_H_

#include "tbsys.h"
#include "common/ob_ms_list.h"
#include "common/ob_server_config.h"
#include "common/ob_system_config.h"

using namespace oceanbase;

namespace oceanbase
{
  namespace common
  {
    class ObConfigManager
    {
        friend class UpdateTask;
      public:
        ObConfigManager(ObServerConfig &server_config);
        ~ObConfigManager();

        /* get newest version received */
        const int64_t& get_version() const;
        /* config version current used */
        int64_t get_current_version() const;

        int base_init();
        int init(const ObServer &server,
                 const ObClientManager &client_mgr,
                 ObTimer &timer);
        int init(MsList &mslist, const ObClientManager &client_mgr, ObTimer &timer);

        /* manual dump to file named by path */
        int dump2file(const char *path = NULL) const;

        /* set dump path (filename) for autodump */
        void set_dump_path(const char *path);

        /* This function should been invoked after @base_init */
        int load_config(const char *path = NULL);

        int update_local();
        int got_version(int64_t version);
      private:
        class UpdateTask
          : public ObTimerTask
        {
          public:
            UpdateTask()
              : config_mgr_(NULL), update_local_(false),
                version_(0), scheduled_time_(0)
            {}

            // main routine
            void runTimerTask(void);
            int write2stat(void);
            ObConfigManager *config_mgr_;
            bool update_local_;
            volatile int64_t version_;
            volatile int64_t scheduled_time_;
        };
        static const int64_t DEFAULT_VERSION = 1;
      private:
        ObTimer *timer_;
        /* Update local config from internal table */
        UpdateTask update_task_;
        ObServer ms_self_;
        const ObClientManager *client_mgr_;
        MsList *mslist_;
        ObSystemConfig system_config_;
        ObServerConfig &server_config_;
        int64_t newest_version_;
        int64_t current_version_;
        char dump_path_[OB_MAX_FILE_NAME_LENGTH];
        tbutil::Mutex mutex_;
    };

    inline ObConfigManager::ObConfigManager(ObServerConfig &server_config)
      : timer_(NULL), server_config_(server_config),
        newest_version_(1), current_version_(1)
    {
    }

    inline ObConfigManager::~ObConfigManager()
    {

    }

    inline int64_t ObConfigManager::get_current_version() const
    {
      return current_version_;
    }

    inline const int64_t& ObConfigManager::get_version() const
    {
      return newest_version_;
    }

    inline void ObConfigManager::set_dump_path(const char *path)
    {
      snprintf(dump_path_, OB_MAX_FILE_NAME_LENGTH, "%s", path);
    }
  }
}

#endif /* _OB_CONFIG_MANAGER_H_ */

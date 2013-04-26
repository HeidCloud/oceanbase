/**
 * (C) 2010-2012 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * ob_row_util.h
 *
 * Authors:
 *   Zhifeng YANG <zhuweng.yzf@taobao.com>
 *
 */
#ifndef _OB_ROW_UTIL_H
#define _OB_ROW_UTIL_H 1
#include "ob_row.h"
#include "ob_rowkey.h"

namespace oceanbase
{
  namespace common
  {
    class ObRowUtil
    {
      public:
        /**
         * convert row to compact_row
         *
         * @param row [in]
         * @param compact_row [in/out]
         *
         * @return OB_SIZE_OVERFLOW if buffer not enough
         */
        static int convert(const ObRow &row, common::ObString &compact_row);

        /**
         * convert row to compact_row, and 
         * save row to new_row which uses compact_row storage
         *
         * @param row [in]
         * @param compact_row [in/out]
         * @param out_row[out]
         * @return OB_SIZE_OVERFLOW if buffer not enough
         */
        static int convert(const ObRow &row, common::ObString &compact_row, ObRow &out_row);

        /**
         * convert compact_row to row
         *
         * @param compact_row [in]
         * @param row [in/out] use row.get_row_desc() as an input param
         *
         * @return
         */
        static int convert(const common::ObString &compact_row, ObRow &row);

        static int convert(const common::ObString &compact_row, ObRow &row, ObRowkey *rowkey, ObObj *rowkey_buf);
    };
  } // end namespace common
} // end namespace oceanbase

#endif /* _OB_ROW_UTIL_H */


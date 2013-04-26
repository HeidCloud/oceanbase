/**
 * (C) 2010-2012 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * ob_merge_union.h
 *
 * Authors:
 *   TIAN GUAN <tianguan.dgb@taobao.com>
 *
 */
#ifndef OCEANBASE_SQL_OB_MERGE_UNION_H_
#define OCEANBASE_SQL_OB_MERGE_UNION_H_

#include "sql/ob_set_operator.h"
#include "common/ob_row.h"

namespace oceanbase
{
  namespace sql
  {
    class ObMergeUnion: public ObSetOperator
    {
      public:
        ObMergeUnion();
        virtual ~ObMergeUnion();
        virtual int open();
        virtual int close();
        /**
         * 获得下一行的引用
         * @note 在下次调用get_next或者close前，返回的row有效
         * @pre 调用open()
         * @param row [out]
         *
         * @return OB_SUCCESS或OB_ITER_END或错误码
         */
        virtual int get_next_row(const common::ObRow *&row);
        virtual int get_row_desc(const common::ObRowDesc *&row_desc) const;
        virtual int64_t to_string(char* buf, const int64_t buf_len) const;
        virtual int set_distinct(bool is_distinct);
      private:
        int cons_row_desc();
        int do_distinct(ObPhyOperator *op, const common::ObRow *&row);
        int compare(const common::ObRow *this_row, const common::ObRow *last_row, int &cmp) const;
        int distinct_get_next_row(const common::ObRow *&row);
        int all_get_next_row(const common::ObRow *&row);
        DISALLOW_COPY_AND_ASSIGN(ObMergeUnion);
      private:
        typedef int (ObMergeUnion::*get_next_row_func_type)(const common::ObRow *&row);
        get_next_row_func_type get_next_row_func_;
      private:
        const common::ObRow *cur_first_query_row_;
        const common::ObRow *cur_second_query_row_;
        char *last_row_buf_;
        int left_ret_;
        int right_ret_;
        int last_cmp_;
        bool got_first_row_;
        const common::ObRow *last_output_row_;
        const common::ObRowDesc *right_row_desc_;
        // 前一条已经被输出的ObRow
        common::ObRow last_row_;
        // 当上一行的数据来自从union的第二张表时，需要将第二张表的那一行的row desc置为原值
        // 保存上一次输出的row的指针,只可能指向右表
        static const uint64_t OB_ROW_BUF_SIZE = common::OB_MAX_ROW_LENGTH;
    };
  } // end namespace sql
} // end namespace oceanbase

#endif /* OCEANBASE_SQL_OB_MERGE_UNION_H_ */

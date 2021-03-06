#include "task_binlog_scan.h"
#include "../base.h"

task_binlog_scan_t * task_binlog_scan_t::create ( )
{
    task_binlog_scan_t * p = NULL;

    try
    {
        p = new task_binlog_scan_t ( );
    }
    catch ( ... )
    {
        LOG_ERROR ( "[slow_task][binlog_scan]bad_alloc" );
    }

    return p;
}

void task_binlog_scan_t::release ( )
{
    delete this;
}

task_binlog_scan_t::task_binlog_scan_t ( )
{
}

task_binlog_scan_t::~ task_binlog_scan_t ( )
{
}

void task_binlog_scan_t::process ( )
{
    process_binlog_scan ();
}

void task_binlog_scan_t::process_binlog_scan ( )
{
    try
    {
        i_server_kv_t * db = ( ( hustdb_t * ) G_APPTOOL->get_hustdb () )->get_storage ();
        
        struct check_alive_cb_param_t alive_cb_pm; 
        struct export_cb_param_t      export_cb_pm;
        export_cb_pm.start            = 0;
        export_cb_pm.end              = MAX_BUCKET_NUM;
        export_cb_pm.noval            = false;

        int r = db->binlog_scan ( NULL, NULL, & alive_cb_pm, NULL, & export_cb_pm );
        if ( 0 != r )
        {
            LOG_ERROR ( "[slow_task][binlog_scan]task failed: %d", r );
            return;
        }
    }
    catch ( ... )
    {
        LOG_ERROR ( "[slow_task][binlog_scan]task exception" );
    }
}

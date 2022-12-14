cmake_minimum_required(VERSION 3.16)
project(engine)

set(CMAKE_CXX_STANDARD 14)

include_directories(../../../../../base/hiviewdfx/hilog_lite/interfaces/native/kits/hilog)
include_directories(../../../../../foundation/ai/engine/interfaces)
include_directories(../../../../../foundation/ai/engine/interfaces/kits)
include_directories(../../../../../foundation/ai/engine/interfaces/kits/asr)
include_directories(../../../../../foundation/ai/engine/interfaces/kits/cv/image_classification)
include_directories(../../../../../foundation/ai/engine/services/common)
include_directories(../../../../../foundation/ai/engine/services/common/platform/os_wrapper)
include_directories(../../../../../foundation/ai/engine/services/common/platform/os_wrapper/engine_hal/interfaces)
include_directories(../../../../../foundation/ai/engine/services/common/platform/os_wrapper/feature/interfaces)
include_directories(../../../../../foundation/ai/engine/services/common/platform/os_wrapper/utils)
include_directories(../../../../../foundation/ai/engine/services/common/protocol/plugin_config)
include_directories(../../../../../foundation/ai/engine/services/common/protocol/retcode_inner)
include_directories(../../../../../foundation/ai/engine/services/common/protocol/struct_definition)
include_directories(../../../../../foundation/ai/engine/services/common/utils)
include_directories(../../../../../foundation/ai/engine/services/common/utils/encdec/include)
include_directories(../../../../../foundation/ai/engine/services/common/utils/log)
include_directories(../../../../../foundation/ai/engine/services/server)
include_directories(../../../../../foundation/communication/ipc_lite/frameworks/liteipc/include)
include_directories(../../../../../foundation/communication/ipc_lite/interfaces/kits)
include_directories(../../../../../foundation/distributedschedule/samgr_lite/interfaces/innerkits/registry)
include_directories(../../../../../foundation/distributedschedule/samgr_lite/interfaces/innerkits/samgr)
include_directories(../../../../../foundation/distributedschedule/samgr_lite/interfaces/kits/registry)
include_directories(../../../../../foundation/distributedschedule/samgr_lite/interfaces/kits/samgr)
include_directories(../../../../../third_party/bounds_checking_function/include)
include_directories(../../../../../third_party/iniparser)
include_directories(../../../../../utils/native/lite/include)

add_executable(server
        communication_adapter/include/adapter_wrapper.h
        communication_adapter/include/client_listener_handler.h
        communication_adapter/include/future_listener.h
        communication_adapter/include/sa_async_handler.h
        communication_adapter/include/sa_server_adapter.h
        communication_adapter/source/adapter_wrapper.cpp
        communication_adapter/source/client_listener_handler.cpp
        communication_adapter/source/future_listener.cpp
        communication_adapter/source/sa_async_handler.cpp
        communication_adapter/source/sa_server.c
        communication_adapter/source/sa_server_adapter.cpp
        communication_adapter/source/start_server.c
        plugin/asr/keyword_spotting/include/kws_plugin.h
        plugin/asr/keyword_spotting/source/kws_plugin.cpp
        plugin/cv/image_classification/include/ic_plugin.h
        plugin/cv/image_classification/source/ic_plugin.cpp
        plugin/i_plugin.h
        plugin/i_plugin_callback.h
        plugin_manager/include/aie_plugin_info.h
        plugin_manager/include/i_plugin_manager.h
        plugin_manager/include/plugin.h
        plugin_manager/include/plugin_label.h
        plugin_manager/include/plugin_manager.h
        plugin_manager/source/aie_plugin_info.cpp
        plugin_manager/source/plugin.cpp
        plugin_manager/source/plugin_label.cpp
        plugin_manager/source/plugin_manager.cpp
        server_executor/include/async_msg_handler.h
        server_executor/include/engine.h
        server_executor/include/engine_manager.h
        server_executor/include/engine_worker.h
        server_executor/include/future.h
        server_executor/include/future_factory.h
        server_executor/include/i_async_task_manager.h
        server_executor/include/i_engine_manager.h
        server_executor/include/i_future.h
        server_executor/include/i_future_listener.h
        server_executor/include/i_handler.h
        server_executor/include/i_sync_task_manager.h
        server_executor/include/server_executor.h
        server_executor/include/sync_msg_handler.h
        server_executor/include/task.h
        server_executor/source/async_msg_handler.cpp
        server_executor/source/engine.cpp
        server_executor/source/engine_manager.cpp
        server_executor/source/engine_worker.cpp
        server_executor/source/future.cpp
        server_executor/source/future_factory.cpp
        server_executor/source/server_executor.cpp
        server_executor/source/sync_msg_handler.cpp
)

// -*- C++ -*-
/*!
 * @file  ModuleName.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "ModuleName.h"


double speed;

// Module specification
// <rtc-template block="module_spec">
static const char* modulename_spec[] =
  {
    "implementation_id", "ModuleName",
    "type_name",         "ModuleName",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ModuleName::ModuleName(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_velocityCamIn("velocityCam", m_velocityCam),
    m_inVelocityIn("inVelocity", m_inVelocity),
    m_outVelocityOut("outVelocity", m_outVelocity)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ModuleName::~ModuleName()
{
}



RTC::ReturnCode_t ModuleName::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("velocityCam", m_velocityCamIn);
  addInPort("inVelocity", m_inVelocityIn);
  
  // Set OutPort buffer
  addOutPort("outVelocity", m_outVelocityOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ModuleName::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ModuleName::onActivated(RTC::UniqueId ec_id)
{
    speed = 5;
    m_velocityCam.data = 2;
    m_outVelocity.data.va = 0;
    m_outVelocity.data.vx = 0;
    m_outVelocity.data.vy = 0;
    m_outVelocityOut.write();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ModuleName::onDeactivated(RTC::UniqueId ec_id)
{
    m_outVelocity.data.vy = 0;
    m_outVelocity.data.vx = 0;
    m_outVelocity.data.va = 0;
    m_outVelocityOut.write();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ModuleName::onExecute(RTC::UniqueId ec_id)
{
    if (m_inVelocityIn.isNew()) {
        m_inVelocityIn.read();
        if (m_velocityCamIn.isNew()) {
            m_velocityCamIn.read();
            speed = m_velocityCam.data;
        }

        m_outVelocity.data.vy = 0;
        m_outVelocity.data.vx = (double)(((double)m_inVelocity.data.vx * speed) * 0.1);
        m_outVelocity.data.va = m_inVelocity.data.va;
        m_outVelocityOut.write();

        std::cout << m_inVelocity.data.vx << speed << m_inVelocity.data.va;
    }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ModuleName::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ModuleName::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void ModuleNameInit(RTC::Manager* manager)
  {
    coil::Properties profile(modulename_spec);
    manager->registerFactory(profile,
                             RTC::Create<ModuleName>,
                             RTC::Delete<ModuleName>);
  }
  
};



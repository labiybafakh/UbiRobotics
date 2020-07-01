// -*- C++ -*-
/*!
 * @file  SFMLJoystickToVelocity.cpp
 * @brief SFMLJoystick data convert to Velocity2D
 * @date $Date$
 *
 * $Id$
 */

#include "SFMLJoystickToVelocity.h"

// Module specification
// <rtc-template block="module_spec">
static const char* sfmljoysticktovelocity_spec[] =
  {
    "implementation_id", "SFMLJoystickToVelocity",
    "type_name",         "SFMLJoystickToVelocity",
    "description",       "SFMLJoystick data convert to Velocity2D",
    "version",           "1.0",
    "vendor",            "SSR",
    "category",          "Test",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    "conf.default.gainX", "-0.005",
    "conf.default.gainTh", "-0.005",
    "conf.default.xIndex", "1",
    "conf.default.thIndex", "0",
    // Widget
    "conf.__widget__.debug", "text",
    "conf.__widget__.gainX", "text",
    "conf.__widget__.gainTh", "text",
    "conf.__widget__.xIndex", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SFMLJoystickToVelocity::SFMLJoystickToVelocity(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_inIn("in", m_in),
    m_velocityOut("velocity", m_velocity)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SFMLJoystickToVelocity::~SFMLJoystickToVelocity()
{
}



RTC::ReturnCode_t SFMLJoystickToVelocity::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("in", m_inIn);
  
  // Set OutPort buffer
  addOutPort("velocity", m_velocityOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  bindParameter("gainX", m_gainX, "0.01");
  bindParameter("gainTh", m_gainTh, "0.01");
  bindParameter("xIndex", m_xIndex, "0");
  bindParameter("thIndex", m_thIndex, "1");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SFMLJoystickToVelocity::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SFMLJoystickToVelocity::onDeactivated(RTC::UniqueId ec_id)
{
	m_velocity.data.vx = 0;
	m_velocity.data.vy = 0;
	m_velocity.data.va = 0;
	m_velocityOut.write();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SFMLJoystickToVelocity::onExecute(RTC::UniqueId ec_id)
{
	if(m_inIn.isNew()) {
		m_inIn.read();

		if(m_in.data.length() <= m_xIndex || m_in.data.length() <= m_thIndex) {
			std::cout << "[RTC::SFMLJoystickToVelocity] Too short Sequence Input." << std::endl;
			return RTC::RTC_ERROR;
		}

		m_velocity.data.vx = m_gainX * m_in.data[m_xIndex];
		m_velocity.data.vy = 0;
		m_velocity.data.va = m_gainTh * m_in.data[m_thIndex];
		m_velocityOut.write();

		if(m_debug) {
			std::cout << "[RTC::SFMLJoystickToVelocity] Velocity = " << m_velocity.data.vx << ", " << m_velocity.data.vy << ", " << m_velocity.data.va << std::endl;
		}
	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystickToVelocity::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SFMLJoystickToVelocityInit(RTC::Manager* manager)
  {
    coil::Properties profile(sfmljoysticktovelocity_spec);
    manager->registerFactory(profile,
                             RTC::Create<SFMLJoystickToVelocity>,
                             RTC::Delete<SFMLJoystickToVelocity>);
  }
  
};



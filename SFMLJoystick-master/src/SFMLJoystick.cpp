// -*- C++ -*-
/*!
 * @file  SFMLJoystick.cpp
 * @brief Joystick Component using SFML Library
 * @date $Date$
 *
 * @author Sugar Sweet Robotics
 *
 * LGPL
 *
 * $Id$
 */

#include "SFMLJoystick.h"

#include <SFML/Window/Joystick.hpp>

// Module specification
// <rtc-template block="module_spec">
static const char* sfmljoystick_spec[] =
  {
    "implementation_id", "SFMLJoystick",
    "type_name",         "SFMLJoystick",
    "description",       "Joystick Component using SFML Library",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "HumanInterface",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "15",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "1",
    "conf.default.id", "0",
    "conf.default.axis_max", "100", 
    "conf.default.buttonOutputStyle", "event",
    // Widget
    "conf.__widget__.debug", "text",
    "conf.__widget__.id", "text",
    "conf.__widget__.buttonOutputStyle", "spin",
    // Constraints
    "conf.__constraints__.buttonOutputStyle", "event, continuous",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SFMLJoystick::SFMLJoystick(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_axisOut("axis", m_axis),
    m_buttonsOut("buttons", m_buttons)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SFMLJoystick::~SFMLJoystick()
{
}



RTC::ReturnCode_t SFMLJoystick::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("axis", m_axisOut);
  addOutPort("buttons", m_buttonsOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  bindParameter("id", m_id, "0");
  bindParameter("axis_max", m_axis_max, "100");
  bindParameter("buttonOutputStyle", m_buttonOutputStyle, "event");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SFMLJoystick::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystick::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystick::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SFMLJoystick::onActivated(RTC::UniqueId ec_id)
{
  sf::Joystick::update();
  if(!sf::Joystick::isConnected(m_id)) {
    std::cout << "[RTC::SFMLJoystick] Joystick (id=" << m_id << ") not found." << std::endl;
    return RTC::RTC_ERROR;
  }

  m_buttons.data.length(sf::Joystick::getButtonCount(m_id));
  for(size_t i = 0;i < sf::Joystick::getButtonCount(m_id);i++) {
    m_buttons.data[i] = false;
  }
  m_axis.data.length(8);
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SFMLJoystick::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SFMLJoystick::onExecute(RTC::UniqueId ec_id)
{
  sf::Joystick::update();
  if (!sf::Joystick::isConnected(m_id)) {
    std::cout << "[RTC::SFMLJoystick] Joystick (id=" << m_id << ") not found." << std::endl;
    return RTC::RTC_ERROR;
  }

  bool buttonStateUpdated = false;
  for(size_t i = 0;i < sf::Joystick::getButtonCount(m_id);i++) {
    m_buttonsBuffer[i] = sf::Joystick::isButtonPressed(m_id, i);
    buttonStateUpdated |= m_buttonsBuffer[i] != m_buttons.data[i];
    m_buttons.data[i] = m_buttonsBuffer[i];
  }

  if(m_buttonOutputStyle == "continuous" || buttonStateUpdated) {
    m_buttonsOut.write();
  }
  
  m_axis.data[0] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::X);
  m_axis.data[1] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::Y);
  m_axis.data[2] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::Z);

  m_axis.data[3] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::R);
  m_axis.data[4] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::U);
  m_axis.data[5] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::V);
  
  m_axis.data[6] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::PovX);
  m_axis.data[7] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::PovY);

  for(int i = 0;i < 8;i++) {
    m_axis.data[i] = (int)(m_axis.data[i] / 100.0 * m_axis_max);
  }
  m_axisOut.write();


  if(m_debug) {
    std::cout << "AXIS:    X,    Y,    Z,    R,    U,    V,  PovX,  PovY, \n";
    std::cout.width(4);
    std::cout << "     ";
    for(int i = 0;i < 8;i++) {
      std::cout << m_axis.data[i] << ", ";
    }
    
    std::cout << "\nBTN :";
    for(size_t i = 0;i < sf::Joystick::getButtonCount(m_id);i++) {
      std::cout << m_buttons.data[i];
    }
    std::cout << std::endl;
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SFMLJoystick::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystick::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystick::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystick::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SFMLJoystick::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SFMLJoystickInit(RTC::Manager* manager)
  {
    coil::Properties profile(sfmljoystick_spec);
    manager->registerFactory(profile,
                             RTC::Create<SFMLJoystick>,
                             RTC::Delete<SFMLJoystick>);
  }
  
};



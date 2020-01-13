#pragma once
#include "api/machine.hpp"
#include "machines/button.hpp"
#include "machines/keyboard.hpp"
#include "machines/LED.hpp"
#include "machines/ptm_monitor.hpp"
#include "machines/processing_unit.hpp"

#include "api/machine_list.tpp"

#define MACHINE_TYPES LUINT::Machines::Button, LUINT::Machines::KeyboardRelay, LUINT::Machines::LED, LUINT::Machines::PTMMonitor, LUINT::Machines::ProcessingUnit

namespace LUINT::Machines
{
	inline LUINT::Machines::List::MachineList<MACHINE_TYPES> allMachineTypes;
}
#pragma once
#include "machine.h"
#include "button.h"
#include "keyboard.h"
#include "LED.h"
#include "ptm_monitor.h"
#include "processing_unit.h"

#include "machine_list.tpp"

#define MACHINE_TYPES LUINT::Machines::Button, LUINT::Machines::KeyboardRelay, LUINT::Machines::LED, LUINT::Machines::PTMMonitor, LUINT::Machines::ProcessingUnit

namespace LUINT::Machines
{
	inline LUINT::Machines::List::MachineList<MACHINE_TYPES> allMachineTypes;
}
// Ring0Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "..\Driverdll\Driverdll.h"
#include "..\hardware assistant\cpu\CPU.h"
#include "..\hardware assistant\disk\diskdefination.h"
#include "..\hardware assistant\gpu\gpu.h"
#include "..\hardware assistant\acpi\ACPI.h"
#include "..\hardware assistant\Audio\CAudio.h"
#include "..\hardware assistant\network\NetworkLib.h"
#include "..\hardware assistant\edid\EDID.h"
#include "..\hardware assistant\smbios\smbioslib.h"
#include "..\hardware assistant\usb\Iusb.h"
#include "..\hardware assistant\superio\Superio.h"

using namespace SV_ASSIST;
using namespace std;

int main()
{
	int te = 0;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	
	ULONG base = 0;
	Ring0::GetSMbusBaseAddr(0x8086, base);

 	DDR3_INFO data[DIMMSlotNum] = {};
 	for (int i = 0; i < 4; i++)
 		Ring0::ReadSPD(base , DIMMADDR[i], data[i]);

	system("cls");
	auto pci = Ring0::GetAllPciInfo();
	for (const auto& item : pci.pciconfig)
	{
		std::string Vendor, Device;
		ULONG  bus,dev, func;
		Ring0::PCIstringToBDF(item.first, bus, dev, func);
		Ring0::GetPCIDeviceName(item.second.VendorID, item.second.DeviceID,Vendor, Device);
		cout << hex << "Bus:" << bus << " Dev:" << dev << " Func:" << func << "	" << Vendor << " " << Device << endl;
	}

	AUDIO::UpdateData();
	auto audio = AUDIO::GetOutputAudio();

	auto sioid = SUPERIO::GetSuperIoChipId();

	auto siopage = SUPERIO::GetSioPage(sioid);
	auto siodata = SUPERIO::GetSioSensor();
	std::vector<Super_IO_Data> iqwe;
	SUPERIO::ReadSioPage(iqwe, siopage);

	auto cpuname = CPU::GetCPUName();
	auto busspeed = CPU::GetExtClock();
	auto cpucache = CPU::GetCache();
	auto south = CPU::GetSouthbridgeName();
	CPU::Updatedata();

	auto cpufre = CPU::GetCurrentClockSpeed();

	int i = 4;
	while (--i)
	{
		Net::Update();
		auto net = Net::GetData();
		SUPERIO::UpdateData();
		CPU::Updatedata();
		cout << CPU::GetExtClock() << endl;
	}

	Display::UpdateData();
	auto edid = Display::GetEDID();

	GPU::UpdateDate();
	auto gpu = GPU::GetGpuInfo();

	return 0;
}


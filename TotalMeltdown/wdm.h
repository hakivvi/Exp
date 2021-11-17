#pragma once

typedef LARGE_INTEGER PHYSICAL_ADDRESS, * PPHYSICAL_ADDRESS;
#pragma pack(push,4)
typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
	UCHAR Type;
	UCHAR ShareDisposition;
	USHORT Flags;
	union {
		struct {
			PHYSICAL_ADDRESS Start;
			ULONG Length;
		} Generic;

		struct {
			PHYSICAL_ADDRESS Start;
			ULONG Length;
		} Port;

		struct {
#if defined(NT_PROCESSOR_GROUPS)
			USHORT Level;
			USHORT Group;
#else
			ULONG Level;
#endif
			ULONG Vector;
			KAFFINITY Affinity;
		} Interrupt;

		struct {
			union {
				struct {
#if defined(NT_PROCESSOR_GROUPS)
					USHORT Group;
#else
					USHORT Reserved;
#endif
					USHORT MessageCount;
					ULONG Vector;
					KAFFINITY Affinity;
				} Raw;

				struct {
#if defined(NT_PROCESSOR_GROUPS)
					USHORT Level;
					USHORT Group;
#else
					ULONG Level;
#endif
					ULONG Vector;
					KAFFINITY Affinity;
				} Translated;
			} DUMMYUNIONNAME;
		} MessageInterrupt;

		struct {
			PHYSICAL_ADDRESS Start;
			ULONG Length;
		} Memory;

		struct {
			ULONG Channel;
			ULONG Port;
			ULONG Reserved1;
		} Dma;

		struct {
			ULONG Channel;
			ULONG RequestLine;
			UCHAR TransferWidth;
			UCHAR Reserved1;
			UCHAR Reserved2;
			UCHAR Reserved3;
		} DmaV3;

		struct {
			ULONG Data[3];
		} DevicePrivate;

		struct {
			ULONG Start;
			ULONG Length;
			ULONG Reserved;
		} BusNumber;

		struct {
			ULONG DataSize;
			ULONG Reserved1;
			ULONG Reserved2;
		} DeviceSpecificData;

		struct {
			PHYSICAL_ADDRESS Start;
			ULONG Length40;
		} Memory40;

		struct {
			PHYSICAL_ADDRESS Start;
			ULONG Length48;
		} Memory48;

		struct {
			PHYSICAL_ADDRESS Start;
			ULONG Length64;
		} Memory64;

		struct {
			UCHAR Class;
			UCHAR Type;
			UCHAR Reserved1;
			UCHAR Reserved2;
			ULONG IdLowPart;
			ULONG IdHighPart;
		} Connection;

	} u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, * PCM_PARTIAL_RESOURCE_DESCRIPTOR;
#pragma pack(pop,4)

typedef enum _INTERFACE_TYPE {
	InterfaceTypeUndefined,
	Internal,
	Isa,
	Eisa,
	MicroChannel,
	TurboChannel,
	PCIBus,
	VMEBus,
	NuBus,
	PCMCIABus,
	CBus,
	MPIBus,
	MPSABus,
	ProcessorInternal,
	InternalPowerBus,
	PNPISABus,
	PNPBus,
	Vmcs,
	ACPIBus,
	MaximumInterfaceType
} INTERFACE_TYPE, * PINTERFACE_TYPE;

typedef struct _CM_PARTIAL_RESOURCE_LIST {
	USHORT                         Version;
	USHORT                         Revision;
	ULONG                          Count;
	CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, * PCM_PARTIAL_RESOURCE_LIST;

typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
	INTERFACE_TYPE           InterfaceType;
	ULONG                    BusNumber;
	CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} *PCM_FULL_RESOURCE_DESCRIPTOR, CM_FULL_RESOURCE_DESCRIPTOR;

typedef struct _CM_RESOURCE_LIST {
	ULONG                       Count;
	CM_FULL_RESOURCE_DESCRIPTOR List[1];
} *PCM_RESOURCE_LIST, CM_RESOURCE_LIST;
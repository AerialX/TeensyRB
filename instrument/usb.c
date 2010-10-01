#define USB_PRIVATE_INCLUDE
#include "usb.h"

/* -------------------------------
   ---- SELECT  ONE  PLATFORM ----
   ------------------------------- */
#define PLATFORM_PS3
//#define PLATFORM_WII

/* -------------------------------
   ---- SELECT ONE INSTRUMENT ----
   ------------------------------- */
//#define INSTRUMENT_GUITAR
#define INSTRUMENT_GUITAR2
//#define INSTRUMENT_DRUMS
//#define INSTRUMENT_DRUMS2
//#define INSTRUMENT_MIC

#define STR_MANUFACTURER	L"AerialX"
#define STR_PRODUCT		L"Rock Band Instrument"

#ifdef PLATFORM_PS3
#define VENDOR_ID	0x12BA
#endif
#ifdef PLATFORM_WII
#define VENDOR_ID	0x1BAD
#endif

#ifdef INSTRUMENT_GUITAR
	#ifdef PLATFORM_PS3
		#define PRODUCT_ID	0x0100
	#endif
	#ifdef PLATFORM_WII
		#define PRODUCT_ID	0x3000
	#endif
#endif
#ifdef INSTRUMENT_GUITAR2
	#ifdef PLATFORM_PS3
		#define PRODUCT_ID	0x0200
	#endif
	#ifdef PLATFORM_WII
		#define PRODUCT_ID	0x3010
	#endif
#endif
#ifdef INSTRUMENT_DRUMS
	#ifdef PLATFORM_PS3
		#define PRODUCT_ID	0x0120
	#endif
	#ifdef PLATFORM_WII
		#define PRODUCT_ID	0x3100
	#endif
#endif
#ifdef INSTRUMENT_DRUMS2
	#ifdef PLATFORM_PS3
		#define PRODUCT_ID	0x0210
	#endif
	#ifdef PLATFORM_WII
		#define PRODUCT_ID	0x3110
	#endif
#endif

#define RAWHID_TX_SIZE		0x06	// transmit packet size
#define RAWHID_TX_INTERVAL	10	// max # of ms between transmit packets
#define RAWHID_RX_SIZE		0x04	// receive packet size
#define RAWHID_RX_INTERVAL	1	// max # of ms between receive packets

#define ENDPOINT0_SIZE		32
#define RAWHID_INTERFACE	0
#define RAWHID_TX_ENDPOINT	1
#define RAWHID_RX_ENDPOINT	2

#if defined(__AVR_AT90USB162__)
#define RAWHID_TX_BUFFER	EP_SINGLE_BUFFER
#define RAWHID_RX_BUFFER	EP_SINGLE_BUFFER
#else
#define RAWHID_TX_BUFFER	EP_DOUBLE_BUFFER
#define RAWHID_RX_BUFFER	EP_DOUBLE_BUFFER
#endif

static const uint8_t PROGMEM endpoint_config_table[] = {
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(RAWHID_TX_SIZE) | RAWHID_TX_BUFFER,
	1, EP_TYPE_INTERRUPT_OUT,  EP_SIZE(RAWHID_RX_SIZE) | RAWHID_RX_BUFFER,
	0,
	0
};


static uint8_t PROGMEM device_descriptor[] = {
	18,					// bLength
	1,					// bDescriptorType
	0x00, 0x02,				// bcdUSB
	0,					// bDeviceClass
	0,					// bDeviceSubClass
	0,					// bDeviceProtocol
	ENDPOINT0_SIZE,				// bMaxPacketSize0
	LSB(VENDOR_ID), MSB(VENDOR_ID),		// idVendor
	LSB(PRODUCT_ID), MSB(PRODUCT_ID),	// idProduct
	0x13, 0x03,				// bcdDevice
	1,					// iManufacturer
	2,					// iProduct
	0,					// iSerialNumber
	1					// bNumConfigurations
};

// http://sites.google.com/site/infnorm/rbguitartechnicaldetails
static uint8_t PROGMEM rawhid_hid_report_desc[] = {
	0x05,0x01, // usage page (generic desktop)
	0x09,0x05, // Usage (Gamepad)
	0xa1,0x01, // Collection (Application)
	0x15,0x00, // Logical minimum (0)
	0x25,0x01, // Logical maximum (1)
	0x35,0x00, // Physical minimum (0)
	0x45,0x01, // Physical maximum (1)
	0x75,0x01, // Report size (1)
	0x95,0x0d, // Report Count (13)
	0x05,0x09, // Usage page (button)
	0x19,0x01, // Usage Minimum (button 1)
	0x29,0x0d, // Usage Maximum (button 13)
	0x81,0x02, // Input (Data, Variable, Abs)
	0x95,0x03, // Report size (3)
	0x81,0x01, // Input (data,var,abs)
	0x05,0x01, // Usage Page (Generic Desktop)
	0x25,0x07, // Logical Minimum (7)
	0x46,0x3b,0x01, // physical max 315
	0x75,0x04, // report size (4)
	0x95,0x01, // report count (1)
	0x65,0x14, // Unit (eng rot:angular pos),
	0x09,0x39, // Usage (Hat switch)
	0x81,0x42, // INPUT (Data,Var,Abs,Null)
	0x65,0x00,  // unit (none)
	0x95,0x01, // report count 1
	0x81,0x01,// Input (Cnst, Ary, Abs)
	0x26,0xff,0x00, // logical maximum 255
	0x46,0xff,0x00, // physical maximum 255
	0x09,0x30, // usage x
	0x09,0x31, // usage y
	0x09,0x32, // usage z
	0x09,0x35, // usage Rz
	0x75,0x08, // report size 8
	0x95,0x04, // report count 4
	0x81,0x02, // Input (Data,Var,Abs)
	0xC0
};

/*
// lynx44: http://www.rockband.com/forums/showpost.php?p=374540&postcount=24
static uint8_t PROGMEM rawhid_hid_report_desc[] = {
	0x05, 0x01, // Usage Page (Generic Desktop)
	0x15, 0x00, // Logical Minimum (0)
	0x09, 0x04, // Usage (Joystick)
	0xA1, 0x01, // Collection (Application)
	0x05, 0x02, // Usage Page (Simulation Controls)
	0x09, 0xBB, // Usage (Throttle)
	0x15, 0x81, // Logical Minimum (-127)
	0x25, 0x7F, // Logical Maximum (127)
	0x75, 0x08, // Report Size (8)
	0x95, 0x01, // Report Count (3)
	0x81, 0x02, // Input (Data, Variable, Absolute)
	0x05, 0x01, // Usage Page (Generic Desktop)
	0x09, 0x01, // Usage (Pointer)
	0xA1, 0x00, // Collection (Physical)
	0x09, 0x30, // Usage (x)
	0x09, 0x31, // Usage (y)
	0x95, 0x02, // Report Count (2)
	0x81, 0x02, // Input (data,var,abs)
	0xC0, // End Collection
	0x09, 0x39, // Usage (Hat switch)
	0x15, 0x00, // Logical Minimum (0)
	0x25, 0x03, // Logical Maximum (3)
	0x35, 0x00, // Physical Minimum (0)
	0x46, 0x0E, 0x01, //Physical Maximum (270)
	0x65, 0x14, // Unit (eng rot:angular pos)
	0x75, 0x04, // Report Size (4)
	0x95, 0x01, // Report Count (1)
	0x81, 0x02, // Input (data,var,abs)
	0x05, 0x09, // Usage Page (button)
	0x19, 0x01, // Usage Minimum (button 1)
	0x29, 0x04, // Usage Maximum (button 4)
	0x15, 0x00, // Logical Minimum (0)
	0x25, 0x01, // Logical Maximum (1)
	0x75, 0x01, // Report Size (1)
	0x95, 0x04, // Report Count (4)
	0x55, 0x00, // Unit Exponent (0)
	0x65, 0x00, // Unit (none)
	0x81, 0x02, // Input (Data, Variable, Abs)
	0xC0
};
*/

/*static uint8_t PROGMEM rawhid_hid_report_desc[] = {
	0x06, LSB(RAWHID_USAGE_PAGE), MSB(RAWHID_USAGE_PAGE),
	0x0A, LSB(RAWHID_USAGE), MSB(RAWHID_USAGE),
	0xA1, 0x01,				// Collection 0x01
	0x75, 0x08,				// report size = 8 bits
	0x15, 0x00,				// logical minimum = 0
	0x26, 0xFF, 0x00,			// logical maximum = 255
	0x95, RAWHID_TX_SIZE,			// report count
	0x09, 0x01,				// usage
	0x81, 0x02,				// Input (array)
	0x95, RAWHID_RX_SIZE,			// report count
	0x09, 0x02,				// usage
	0x91, 0x02,				// Output (array)
	0xC0					// end collection
};*/


#define CONFIG1_DESC_SIZE        (9+9+9+7+7)
#define RAWHID_HID_DESC_OFFSET   (9+9)

// rbguitartechnicaldetails
static uint8_t PROGMEM config1_descriptor[CONFIG1_DESC_SIZE] = {
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9, 					// bLength;
	2,					// bDescriptorType;
	LSB(CONFIG1_DESC_SIZE),			// wTotalLength
	MSB(CONFIG1_DESC_SIZE),
	1,					// bNumInterfaces
	1,					// bConfigurationValue
	0,					// iConfiguration
	0x80,					// bmAttributes
	50,					// bMaxPower

	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	9,					// bLength
	4,					// bDescriptorType
	RAWHID_INTERFACE,			// bInterfaceNumber
	0,					// bAlternateSetting
	2,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x00,					// bInterfaceSubClass (0x01 = Boot)
	0x00,					// bInterfaceProtocol (0x01 = Keyboard)
	0,					// iInterface
	// HID interface descriptor, HID 1.11 spec, section 6.2.1
	9,					// bLength
	0x21,					// bDescriptorType
	0x10, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(rawhid_hid_report_desc),		// wDescriptorLength
	0,
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	0x81,		// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	0x40, 0,
	RAWHID_TX_INTERVAL,			// bInterval
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	0x02,			// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	0x40, 0,
	RAWHID_RX_INTERVAL			// bInterval
};
/*static uint8_t PROGMEM config1_descriptor[CONFIG1_DESC_SIZE] = {
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9, 					// bLength;
	2,					// bDescriptorType;
	LSB(CONFIG1_DESC_SIZE),			// wTotalLength
	MSB(CONFIG1_DESC_SIZE),
	1,					// bNumInterfaces
	1,					// bConfigurationValue
	0,					// iConfiguration
	0xC0,					// bmAttributes
	50,					// bMaxPower

	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	9,					// bLength
	4,					// bDescriptorType
	RAWHID_INTERFACE,			// bInterfaceNumber
	0,					// bAlternateSetting
	2,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x00,					// bInterfaceSubClass (0x01 = Boot)
	0x00,					// bInterfaceProtocol (0x01 = Keyboard)
	0,					// iInterface
	// HID interface descriptor, HID 1.11 spec, section 6.2.1
	9,					// bLength
	0x21,					// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(rawhid_hid_report_desc),		// wDescriptorLength
	0,
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	RAWHID_TX_ENDPOINT | 0x80,		// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
//	RAWHID_TX_SIZE, 0,			// wMaxPacketSize
	0x40, 0,
	RAWHID_TX_INTERVAL,			// bInterval
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	RAWHID_RX_ENDPOINT,			// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
//	RAWHID_RX_SIZE, 0,			// wMaxPacketSize
	0x40, 0,
	RAWHID_RX_INTERVAL			// bInterval
};*/

struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	int16_t wString[];
};
static struct usb_string_descriptor_struct PROGMEM string0 = {
	4,
	3,
	{0x0409}
};
static struct usb_string_descriptor_struct PROGMEM string1 = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
static struct usb_string_descriptor_struct PROGMEM string2 = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};

static struct descriptor_list_struct {
	uint16_t	wValue;
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint8_t		length;
} PROGMEM descriptor_list[] = {
	{0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
	{0x0200, 0x0000, config1_descriptor, sizeof(config1_descriptor)},
	{0x2200, RAWHID_INTERFACE, rawhid_hid_report_desc, sizeof(rawhid_hid_report_desc)},
	{0x2100, RAWHID_INTERFACE, config1_descriptor+RAWHID_HID_DESC_OFFSET, 9},
	{0x0300, 0x0000, (const uint8_t *)&string0, 4},
	{0x0301, 0x0409, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x0302, 0x0409, (const uint8_t *)&string2, sizeof(STR_PRODUCT)}
};
#define NUM_DESC_LIST (sizeof(descriptor_list)/sizeof(struct descriptor_list_struct))


/**************************************************************************
 *
 *  Variables - these are the only non-stack RAM usage
 *
 **************************************************************************/

// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration=0;

// these are a more reliable timeout than polling the
// frame counter (UDFNUML)
static volatile uint8_t rx_timeout_count=0;
static volatile uint8_t tx_timeout_count=0;



/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/


// initialize USB
void usb_init(void)
{
	HW_CONFIG();
	USB_FREEZE();				// enable USB
	PLL_CONFIG();				// config PLL
        while (!(PLLCSR & (1<<PLOCK))) ;	// wait for PLL lock
        USB_CONFIG();				// start USB clock
        UDCON = 0;				// enable attach resistor
	usb_configuration = 0;
        UDIEN = (1<<EORSTE)|(1<<SOFE);
	sei();
}

// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void)
{
	return usb_configuration;
}


// receive a packet, with timeout
int8_t usb_rawhid_recv(uint8_t *buffer, uint8_t timeout)
{
	uint8_t intr_state;

	// if we're not online (enumerated and configured), error
	if (!usb_configuration) return -1;
	intr_state = SREG;
	cli();
	rx_timeout_count = timeout;
	UENUM = RAWHID_RX_ENDPOINT;
	// wait for data to be available in the FIFO
	while (1) {
		if (UEINTX & (1<<RWAL)) break;
		SREG = intr_state;
		if (rx_timeout_count == 0) return 0;
		if (!usb_configuration) return -1;
		intr_state = SREG;
		cli();
		UENUM = RAWHID_RX_ENDPOINT;
	}

	for (uint8_t i = 0; i < RAWHID_RX_SIZE; i++)
		*buffer++ = UEDATX;
	// release the buffer
	UEINTX = 0x6B;
	SREG = intr_state;
	return RAWHID_RX_SIZE;
}

// send a packet, with timeout
int8_t usb_rawhid_send(const uint8_t *buffer, uint8_t timeout)
{
	uint8_t intr_state;

	// if we're not online (enumerated and configured), error
	if (!usb_configuration) return -1;
	intr_state = SREG;
	cli();
	tx_timeout_count = timeout;
	UENUM = RAWHID_TX_ENDPOINT;
	// wait for the FIFO to be ready to accept data
	while (1) {
		if (UEINTX & (1<<RWAL)) break;
		SREG = intr_state;
		if (tx_timeout_count == 0) return 0;
		if (!usb_configuration) return -1;
		intr_state = SREG;
		cli();
		UENUM = RAWHID_TX_ENDPOINT;
	}
	for (uint8_t i = 0; i < RAWHID_TX_SIZE; i++)
		UEDATX = *buffer++;
	// transmit it now
	UEINTX = 0x3A;
	SREG = intr_state;
	return RAWHID_TX_SIZE;
}




/**************************************************************************
 *
 *  Private Functions - not intended for general user consumption....
 *
 **************************************************************************/


#if (GCC_VERSION >= 40300) && (GCC_VERSION < 40302)
#error "Buggy GCC 4.3.0 compiler, please upgrade!"
#endif


// USB Device Interrupt - handle all device-level events
// the transmit buffer flushing is triggered by the start of frame
//
ISR(USB_GEN_vect)
{
	uint8_t intbits, t;

        intbits = UDINT;
        UDINT = 0;
        if (intbits & (1<<EORSTI)) {
		UENUM = 0;
		UECONX = 1;
		UECFG0X = EP_TYPE_CONTROL;
		UECFG1X = EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER;
		UEIENX = (1<<RXSTPE);
		usb_configuration = 0;
        }
	if ((intbits & (1<<SOFI)) && usb_configuration) {
		t = rx_timeout_count;
		if (t) rx_timeout_count = --t;
		t = tx_timeout_count;
		if (t) tx_timeout_count = --t;
	}
}



// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void)
{
	while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void)
{
	while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void)
{
	UEINTX = ~(1<<RXOUTI);
}



// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
//
ISR(USB_COM_vect)
{
        uint8_t intbits;
	const uint8_t *list;
        const uint8_t *cfg;
	uint8_t i, n, len, en;
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
	uint16_t desc_val;
	const uint8_t *desc_addr;
	uint8_t	desc_length;

        UENUM = 0;
	intbits = UEINTX;
        if (intbits & (1<<RXSTPI)) {
                bmRequestType = UEDATX;
                bRequest = UEDATX;
                wValue = UEDATX;
                wValue |= (UEDATX << 8);
                wIndex = UEDATX;
                wIndex |= (UEDATX << 8);
                wLength = UEDATX;
                wLength |= (UEDATX << 8);
                UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
                if (bRequest == GET_DESCRIPTOR) {
			list = (const uint8_t *)descriptor_list;
			for (i=0; ; i++) {
				if (i >= NUM_DESC_LIST) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);  //stall
					return;
				}
				desc_val = pgm_read_word(list);
				if (desc_val != wValue) {
					list += sizeof(struct descriptor_list_struct);
					continue;
				}
				list += 2;
				desc_val = pgm_read_word(list);
				if (desc_val != wIndex) {
					list += sizeof(struct descriptor_list_struct)-2;
					continue;
				}
				list += 2;
				desc_addr = (const uint8_t *)pgm_read_word(list);
				list += 2;
				desc_length = pgm_read_byte(list);
				break;
			}
			len = (wLength < 256) ? wLength : 255;
			if (len > desc_length) len = desc_length;
			do {
				// wait for host ready for IN packet
				do {
					i = UEINTX;
				} while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
				if (i & (1<<RXOUTI)) return;	// abort
				// send IN packet
				n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
				for (i = n; i; i--) {
					UEDATX = pgm_read_byte(desc_addr++);
				}
				len -= n;
				usb_send_in();
			} while (len || n == ENDPOINT0_SIZE);
			return;
                }
		if (bRequest == SET_ADDRESS) {
			usb_send_in();
			usb_wait_in_ready();
			UDADDR = wValue | (1<<ADDEN);
			return;
		}
		if (bRequest == SET_CONFIGURATION && bmRequestType == 0) {
			usb_configuration = wValue;
			usb_send_in();
			cfg = endpoint_config_table;
			for (i=1; i<5; i++) {
				UENUM = i;
				en = pgm_read_byte(cfg++);
				UECONX = en;
				if (en) {
					UECFG0X = pgm_read_byte(cfg++);
					UECFG1X = pgm_read_byte(cfg++);
				}
			}
        		UERST = 0x1E;
        		UERST = 0;
			return;
		}
		if (bRequest == GET_CONFIGURATION && bmRequestType == 0x80) {
			usb_wait_in_ready();
			UEDATX = usb_configuration;
			usb_send_in();
			return;
		}

		if (bRequest == GET_STATUS) {
			usb_wait_in_ready();
			i = 0;
			if (bmRequestType == 0x82) {
				UENUM = wIndex;
				if (UECONX & (1<<STALLRQ)) i = 1;
				UENUM = 0;
			}
			UEDATX = i;
			UEDATX = 0;
			usb_send_in();
			return;
		}
		if ((bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE)
		  && bmRequestType == 0x02 && wValue == 0) {
			i = wIndex & 0x7F;
			if (i >= 1 && i <= MAX_ENDPOINT) {
				usb_send_in();
				UENUM = i;
				if (bRequest == SET_FEATURE) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);
				} else {
					UECONX = (1<<STALLRQC)|(1<<RSTDT)|(1<<EPEN);
					UERST = (1 << i);
					UERST = 0;
				}
				return;
			}
		}
		if (wIndex == RAWHID_INTERFACE) {
			if (bmRequestType == 0xA1 && bRequest == HID_GET_REPORT) {
				len = RAWHID_TX_SIZE;
				do {
					// wait for host ready for IN packet
					do {
						i = UEINTX;
					} while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
					if (i & (1<<RXOUTI)) return;	// abort
					// send IN packet
					n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
					for (i = n; i; i--) {
						// just send zeros
						UEDATX = 0;
					}
					len -= n;
					usb_send_in();
				} while (len || n == ENDPOINT0_SIZE);
				return;
			}
			if (bmRequestType == 0x21 && bRequest == HID_SET_REPORT) {
				len = RAWHID_RX_SIZE;
				do {
					n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
					usb_wait_receive_out();
					// ignore incoming bytes
					usb_ack_out();
					len -= n;
				} while (len);
				usb_wait_in_ready();
				usb_send_in();
				return;
			}
		}
	}
	UECONX = (1<<STALLRQ) | (1<<EPEN);	// stall
}



#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <QObject>
#include <QAbstractTableModel>

class Config;
class RXGroupList;
class DigitalContact;
class ScanList;
class GPSSystem;


/** The base class of all channels (analog and digital) of a codeplug configuration.
 *
 * This class holds the common configuration of @c AnalogChannel and @c DigitalChannel, that is
 * the name, RX and TX frequencies, output power, TOT and default scanlist properties.
 *
 * @ingroup conf */
class Channel: public QObject
{
	Q_OBJECT

public:
  /** Possible power settings. */
	typedef enum {
		HighPower, ///< High power setting (e.g, 5W).
    LowPower   ///< Low power setting (e.g., 1W)
	} Power;

protected:
  /** Hidden constructor.
   * Constructs a new base channel.
   * @param name      Specifies the name of the channel.
   * @param rx        Sepcifies the RX freqeuncy in MHz.
   * @param tx        Specifies the TX frequency in MHz.
   * @param power     Specifies the power setting for the channel.
   * @param txTimeout Specifies the transmit timeout in seconds (TOT).
   * @param rxOnly    Specifies whether the channel is RX only.
   * @param scanlist  Specifies the default scanlist for the channel.
   * @param parent    Specified the @c QObject parent object. */
	Channel(const QString &name, float rx, float tx, Power power, uint txTimeout, bool rxOnly,
	        ScanList *scanlist, QObject *parent=nullptr);

public:
  /** Returns @c true if the channel is of type @c T. This can be used to text wheter this channel
   * is actuially an analog or digital channel: @c channel->is<AnalogChannel>(). */
  template<class T>
  bool is() const {
	  return 0 != dynamic_cast<const T *>(this);
  }

  /** Dynamic cast of channels. For example @c channel->as<AnalogChannel>(). */
  template<class T>
  T *as() {
	  return dynamic_cast<T *>(this);
  }

  /** Dynamic cast of channels. For example @c channel->as<AnalogChannel>(). */
  template<class T>
  const T *as() const{
	  return dynamic_cast<const T *>(this);
  }

  /** Returns the name of the channel. */
  const QString &name() const;
  /** (Re-)Sets the name of the channel. */
  bool setName(const QString &name);

  /** Returns the RX frequency of the channel in MHz. */
  float rxFrequency() const;
  /** (Re-)Sets the RX frequency of the channel in MHz. */
  bool setRXFrequency(float freq);
  /** Returns the TX frequency of the channel in MHz. */
  float txFrequency() const;
  /** (Re-)Sets the TX frequency of the channel in MHz. */
  bool setTXFrequency(float freq);

  /** Returns the power setting of the channel. */
  Power power() const;
  /** (Re-)Sets the power setting of the channel. */
  void setPower(Power power);

  /** Returns the TX timeout (TOT) in seconds. */
  uint txTimeout() const;
  /** (Re-)Sets the TX timeout (TOT) in seconds. */
  bool setTimeout(uint dur);

  /** Returns @c true, if the channel is RX only. */
  bool rxOnly() const;
  /** Set, whether the channel is RX only. */
  bool setRXOnly(bool enable);

  /** Returns the default scan list for the channel. */
  ScanList *scanList() const;
  /** (Re-) Sets the default scan list for the channel. */
  bool setScanList(ScanList *list);

signals:
  /** Is emitted if the channel gets modified. */
  void modified();

protected slots:
  /** Internal hander for deleted scan lists. */
  void onScanListDeleted(QObject *obj);

protected:
  /** The channel name. */
  QString _name;
  /** The RX frequency in MHz. */
  float _rxFreq;
  /** The TX frequency in MHz. */
  float _txFreq;
  /** The transmit power setting. */
  Power _power;
  /** Transmit timeout in seconds. */
  uint  _txTimeOut;
  /** RX only flag. */
  bool  _rxOnly;
  /** Default scan list of the channel. */
  ScanList *_scanlist;
};


/** Extension to the @c Channel class to implement an analog channel.
 *
 * This class implements all the properties specific to an analog channel. That is, the admit
 * criterion, squelch, RX and TX tones and bandwidth settings.
 *
 * @ingroup conf */
class AnalogChannel: public Channel
{
public:
  /** Admit criteria of analog channel. */
	typedef enum {
		AdmitNone,  ///< Allow always.
    AdmitFree,  ///< Allow when channel free.
    AdmitTone   ///< Allow when admit tone is present.
	} Admit;

  /** Possible bandwidth of an analog channel. */
	typedef enum {
		BWNarrow,  ///< Narrow bandwidth (12.5kHz).
    BWWide     ///< Wide bandwidth (25kHz).
	} Bandwidth;

public:
  /** Constructs a new analog channel.
   *
   * @param name      Specifies the name of the channel.
   * @param rxFreq    Sepcifies the RX freqeuncy in MHz.
   * @param txFreq    Specifies the TX frequency in MHz.
   * @param power     Specifies the power setting for the channel.
   * @param txTimeout Specifies the transmit timeout in seconds (TOT).
   * @param rxOnly    Specifies whether the channel is RX only.
   * @param admit     Specifies the admit criterion.
   * @param squelch   Specifies the squelch level [0,10].
   * @param rxTone    Specifies CTCSS RX tone.
   * @param txTone    Specifies CTCSS transmit tone.
   * @param bw        Specifies the bandwidth.
   * @param list      Specifies the default scanlist for the channel.
   * @param parent    Specified the @c QObject parent object. */
	AnalogChannel(const QString &name, float rxFreq, float txFreq, Power power, uint txTimeout,
	              bool rxOnly, Admit admit, uint squelch, float rxTone, float txTone, Bandwidth bw,
                ScanList *list, QObject *parent=nullptr);

  /** Returns the admit criterion for the analog channel. */
	Admit admit() const;
  /** (Re-)Sets the admit criterion for the analog channel. */
	void setAdmit(Admit admit);

  /** Returns the squelch level [0,10]. */
	uint squelch() const;
  /** (Re-)Sets the squelch level [0,10]. 0 Disables squelch (on some radios). */
	bool setSquelch(uint squelch);

  /** Returns the CTCSS RX tone, 0=disabled. */
	float rxTone() const;
  /** (Re-)Sets the CTCSS RX tone, 0 disables the RX tone. */
	bool setRXTone(float freq);
  /** Returns the CTCSS TX tone, 0=disabled. */
	float txTone() const;
  /** (Re-)Sets the CTCSS TX tone, 0 disables the TX tone. */
	bool setTXTone(float freq);

  /** Returns the bandwidth of the analog channel. */
	Bandwidth bandwidth() const;
  /** (Re-)Sets the bandwidth of the analog channel. */
	bool setBandwidth(Bandwidth bw);

protected:
  /** Holds the admit criterion. */
	Admit _admit;
  /** Holds the squelch level [0,10]. */
	uint  _squelch;
  /** The RX CTCSS tone. */
	float _rxTone;
  /** The TX CTCSS tone. */
	float _txTone;
  /** The channel bandwidth. */
	Bandwidth _bw;
};



/** Extension to the @c Channel class to implement an digital (DMR) channel.
 *
 * That is, the admit criterion, color code, time slot, RX group list and TX contact.
 *
 * @ingroup conf */
class DigitalChannel: public Channel
{
	Q_OBJECT

public:
  /** Possible admit criteria of digital channels. */
	typedef enum {
		AdmitNone,      ///< No admit criteria, allows to transmit any time.
    AdmitFree,      ///< Transmit only if channel is free.
    AdmitColorCode  ///< Transmit only if channel is free and matches given color code.
	} Admit;

  /** Possible timeslots for digital channels. */
	typedef enum {
		TimeSlot1, ///< Time/repeater slot 1
    TimeSlot2  ///< Time/repeater slot 2
	} TimeSlot;

public:
  /** Constructs a new digital (DMR) channel.
   *
   * @param name      Specifies the name of the channel.
   * @param rxFreq    Sepcifies the RX freqeuncy in MHz.
   * @param txFreq    Specifies the TX frequency in MHz.
   * @param power     Specifies the power setting for the channel.
   * @param txTimeout Specifies the transmit timeout in seconds (TOT).
   * @param rxOnly    Specifies whether the channel is RX only.
   * @param admit     Specifies the admit criterion.
   * @param colorCode Specifies the colorcode [1,15].
   * @param timeslot  Specifies the time-slot.
   * @param rxGroup   Specifies the RX group list for the channel.
   * @param txContact Specifies the default TX contact to call on this channel.
   * @param gpsSystem Specifies the GPS system to use on this channel.
   * @param list      Specifies the default scanlist for the channel.
   * @param parent    Specified the @c QObject parent object. */
	DigitalChannel(const QString &name, float rxFreq, float txFreq, Power power, uint txTimeout,
	               bool rxOnly, Admit admit, uint colorCode, TimeSlot timeslot, RXGroupList *rxGroup,
                 DigitalContact *txContact, GPSSystem *gpsSystem, ScanList *list, QObject *parent=nullptr);

  /** Returns the admit criterion for the channel. */
	Admit admit() const;
  /** (Re-)Sets the admit criterion for the channel. */
	void setAdmit(Admit admit);

  /** Returns the color code for the channel. */
	uint colorCode() const;
  /** (Re-)Sets the color code for the channel. */
	bool setColorCode(uint cc);

  /** Returns the time slot for the channel. */
	TimeSlot timeslot() const;
  /** (Re-)Sets the time slot for the channel. */
	bool setTimeSlot(TimeSlot ts);

  /** Retruns the RX group list for the channel. */
	RXGroupList *rxGroupList() const;
  /** (Re-)Sets the RX group list for the channel. */
	bool setRXGroupList(RXGroupList *rxg);

  /** Returns the default TX contact to call on this channel. */
	DigitalContact *txContact() const;
  /** (Re-) Sets the default TX contact for this channel. */
	bool setTXContact(DigitalContact *c);

  GPSSystem *gpsSystem() const;
  bool setGPSSystem(GPSSystem *gps);

protected slots:
  /** Internal callback if RX group list is deleted. */
	void onRxGroupDeleted();
  /** Internal callback if TX contact is deleted. */
	void onTxContactDeleted();
  /** Internal callback if GPS system is deleted. */
  void onGPSSystemDeleted();

protected:
  /** The admit criterion. */
	Admit _admit;
  /** The channel color code. */
	uint _colorCode;
  /** The time slot for the channel. */
	TimeSlot _timeSlot;
  /** The RX group list for this channel. */
	RXGroupList *_rxGroup;
  /** The default TX contact. */
	DigitalContact *_txContact;
  /** The GPS system. */
  GPSSystem *_gpsSystem;
};


/** Container class holding all channels (analog and digital) for a specific configuration
 * (@c Config).
 *
 * This class also implements the QAbstractTableModel and can therefore be displayed using a
 * default QTableView instance.
 *
 * @ingroup conf */
class ChannelList: public QAbstractTableModel
{
	Q_OBJECT

public:
  /** Constructs an empty channel list. */
	explicit ChannelList(QObject *parent=nullptr);

  /** Returns the number of channels in this list. */
  int count() const;
  /** Clears the list. */
  void clear();
  /** Returns the index of the channel. */
  int indexOf(Channel *channel) const;
  /** Gets the channel at the specified index. */
	Channel *channel(int idx) const;
  /** Adds a channel to the list at the specified row.
   * If row<0 the channel gets appendet to the list.*/
	int addChannel(Channel *channel, int row=-1);
  /** Removes the given channel from the list. */
	bool remChannel(Channel *channel);
  /** Removes the channel at the given index from the list. */
	bool remChannel(int idx);
  /** Moves the channel at index @c idx one step up. */
  bool moveUp(int idx);
  /** Moves the channel at index @c idx one step up. */
  bool moveDown(int idx);

	// QAbstractTableModel interface
  /** Implements QAbstractTableModel, returns number of rows. */
	int rowCount(const QModelIndex &index) const;
  /** Implements QAbstractTableModel, returns number of colums. */
	int columnCount(const QModelIndex &index) const;
  /** Implements QAbstractTableModel, returns data at cell. */
	QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  /** Implements QAbstractTableModel, returns header at section. */
	QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

signals:
  /** Gets emitted once the table has been changed. */
	void modified();

protected slots:
  /** Internal callback on deleted channels. */
  void onChannelDeleted(QObject *obj);
  /** Internal callback on modified channels. */
  void onChannelEdited();

protected:
  /** Just the vector of channels. */
	QVector<Channel *> _channels;
};


#endif // CHANNEL_HH

#define PALETTESPERBATCH 10

/**
 * Modelizes a packaging command sent by the operator to 
 * the server.
 */
#define PIECE1 1
#define PIECE2 2

typedef struct {
	char num;      // identifier of the batch (timestamp)
	int pieceType; // type of piece produced
	int quantity;  // number of palettes in the batch
	int capacity;  // number of pieces in each box
} MsgPackagingCommand;

/**
 * Modelizes an shipping command sent by the operator to 
 * the server.
 */
typedef struct {
	int orderNum;  // identifier of the order associated with the shipping
	int dock;      // destination dock
	MsgShippingCommandBatch* firstBatch; // first batch in the order
} MsgShippingCommand;

typedef struct {
	int batchNum; // identifier of the batch.
	int quantity; // number of palettes to be taken in the batch
	(struct MsgShippingCommandBatch)* nextBatch; // next batch in the order
} MsgShippingCommandBatch;

/**
 * Modelizes a packaged box beloging to a specific 
 * packaging order (i.e. a batch).
 */
typedef struct {
	int num; // identifier of the box (incremental in a batch, from 1 to quantity)
	MsgPackagingCommand* packagingCommand; // associated packaging command
} MsgBox;

/**
 * Modelizes a palette full of boxes belonging to 
 * a specific packaging order (i.e. a batch).
 */
typedef struct {
	int num; // identifier of a palette (incremental in a batch, from 1 to quantity/PALETTESPERBATCH)
	MsgPackagingCommand* packagingCommand; // associated packaging command
} MsgPalette;

/** 
 * Modelizes a message logged on the disk and sent 
 * to the supervisor.
 */
#define LOGERROR 1   // fatal error waiting for confirmation of the supervisor
#define LOGWARNING 2 // error but the process still continues (non blocking)
#define LOGINFO 3    // informational/confirmation message

typedef struct {
	char type;        // type of the message
	char[64] content; // content the the message
} MsgLog;

/**
 * Modelizes a palette stored in the warehouse. A palette is placed 
 * at an unique location which is an incremental integer (cycling to 
 * get back released spaces).
 */
typedef struct {
	MsgPalette* palette;           // associated palette message
	int location;                  // location of the palette in the warehouse
	(struct Palette)* nextPalette; // next palette in the batch
} Palette;

/**
 * Modelizes a batch stored in the warehouse. Many palettes can 
 * be placed in a single batch. A batch has no location, only 
 * palettes have one.
 */
typedef struct {
	MsgPackagingCommand* packagingCommand; // associated packaging command
	Palette* firstPalette;                 // first palette in the batch
	(struct Batch)* nextBatch;             // next batch in the warehouse
} Batch;

/**
 * Modelizes a warehouse. Many batches can be placed in a warehouse 
 * and each palette in a batch has a physical location.
 */
Batch* warehouse;

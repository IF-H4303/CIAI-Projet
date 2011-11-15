#define PALETTESPARLOT 10

/**
 * Modelizes a packaging command sent by the operator to 
 * the server.
 */
#define PIECE1 1
#define PIECE2 2

typedef struct {
	char num;       // identifier of the batch (timestamp)
	int typePiece;  // type of piece produced
	int quantite;   // number of palettes in the batch
	int contenance; // number of pieces in each box
} MsgOrdreConditionnement;

/**
 * Modelizes an shipping command sent by the operator to 
 * the server.
 */
typedef struct {
	int numCommande;  // identifier of the order associated with the shipping
	int quai;         // destination dock
	MsgOrdreExpeditionLot* premierLot; // first batch in the order
} MsgOrdreExpedition;

typedef struct {
	int numLot;   // identifier of the batch.
	int quantite; // number of palettes to be taken in the batch
	(struct MsgOrdreExpeditionLot)* lotSuivant; // next batch in the order
} MsgOrdreExpeditionLot;

/**
 * Modelizes a packaged box beloging to a specific 
 * packaging order (i.e. a batch).
 */
typedef struct {
	int num; // identifier of the box (incremental in a batch, from 1 to quantity)
	MsgOrdreConditionnement* ordreConditionnement; // associated packaging command
} MsgCarton;

/**
 * Modelizes a palette full of boxes belonging to 
 * a specific packaging order (i.e. a batch).
 */
typedef struct {
	int num; // identifier of a palette (incremental in a batch, from 1 to quantity/PALETTESPERBATCH)
	MsgOrdreConditionnement* ordreConditionnement; // associated packaging command
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
	char[64] contenu; // content the the message
} MsgLog;

/**
 * Modelizes a palette stored in the warehouse. A palette is placed 
 * at an unique location which is an incremental integer (cycling to 
 * get back released spaces).
 */
typedef struct {
	MsgPalette* palette;           // associated palette message
	int emplacement;               // location of the palette in the warehouse
	(struct Palette)* paletteSuivante; // next palette in the batch
} Palette;

/**
 * Modelizes a batch stored in the warehouse. Many palettes can 
 * be placed in a single batch. A batch has no location, only 
 * palettes have one.
 */
typedef struct {
	MsgOrdreConditionnement* ordreConditionnement; // associated packaging command
	Palette* premierePalette;                 // first palette in the batch
	(struct Batch)* lotSuivant;             // next batch in the warehouse
} Lot;

/**
 * Modelizes a warehouse. Many batches can be placed in a warehouse 
 * and each palette in a batch has a physical location.
 */
Lot* entrepot;

//namespace ERROLIST {
char *ERRORSTRING[] = {
    "No Error",                                                      //0
    "An error has occurred. Please refer log file",            //1                                               // 1 //Van233/Jan-2013/AN/Revw/Modified Message
    "Can not get minimum intensity image ",               // 2
    "Please enter valid offset",                                 // 3
    "Please select a series on Iselector",                 // 4
    "Please select Real Image",                                    // 5
    "Please select Magnitude Image",                            // 6
    "Please select Imaginary Image",                            // 7
    "Could not create image series",                             //8
    "Please select more than one image on Iselector",    // 9
    "ROI is placed outside of image region",                                       // 10
    "Please enter Proper ROI data",                             //11
    "Selected protocol does not have raw data",            //12
    "Please enter valid Repetition Time(TR)",                                 //13
    "Can't open study file",                                          //14
    "Failed to retrieve data from VFC"  , //"VfGetFields Error",         //15
    "Different GDC types selected",                                //17
    "Number of images different",                                //18
    "Please select image/s from single protocol",                                //19
    "Please enter a valid positive value",                                //20
    "Please enter a positive integer value",                                //21
    "Selected protocol does not have Real data",                 //22
    "Selected protocol does not have Imaginary data",      //23
    "Selected protocol does not have Magnitude data",         //24
    "File transfer from host to engine failed",             //25
    "Reconstruction failed",                                //26
    "File writing failed",                              //27
    "No ROI has been selected",                                 //28
    "Memory allocation failed",         //29
    "Please select images from a single series on Iselector"    ,   //30
    "No data found to be written to file. Please perform \"Apply\" operation on valid series", //31
    "Please select images for all data types" , //32
    "Data is not proper for fitting" ,          //33
    "Please select more than two images",        //34
    "Please select a ROI  for ROIAVG or INROI operation", //35
    "Value of the parameter is not initialised", //36  NULL value parameter
    "Varfield Data type error",    //37,
    "Please enter offset within range ", //38
    "Please select a valid series on Iselector and perform \"Reselect\"", //39
    "Please select image/s from single series", //40
    "Unable to get ROI average intensity values",  //41
    "Error occurred in ROI Display", //42
    "Invalid Size of buffer for memory allocation", //43
    "Please select a target protocol", //44
    "Please Create the Map File first", //45
    "Dimensions OF Mapfile are incorrect", //46
    "No points extracted from ROIs.\nPlease set different size OR position for the two ROIs.", //47
    "Please select a point inside map region and try again.", //48
    "No points extracted from ROIs. Please select valid ROI positions and perform \"Apply\" ", //49
    "Please enter an odd number in the range 3 to 21 for filter size", //50
    "Please enter smaller filter size for good results (Max.21)",//51
    "Please enter smaller radius value",//52
    "Writing T1-Map to study file failed",                  //53
    "T1Map executable not present.",                        //54
    "Please enter valid initial values",                            //55
    "Please enter initial parameters for selected equation.",  //56
    "Failed to get ROI image map",       //57
    "Failed to load '0x7FE00010'(Image Data) tag from MPlus database.", //58
    "Failed to load '0xE4011008'(Image Scale) tag from MPlus database.", //59
    "Failed to load '0x00180082'(Image time stamp)tag from MPlus database.", //60
	"Failed to load tag from Database", //61
	"Failed to load protocol name from Database", //62
	"Failed to load Series number from Database", //63
    "Unable to Get Data from Database. DB operation failed" ,//64
	"Failed to Write Data to Database", //65
	"Loading image data from Database failed", //66
    "Failed to initialize Socket communication. Please restart TiPrep" ,//67
	"An Exception has occurred.", //68
	"Series Selected is not found in Studyfile.", //69
    "Getting Protocol Number from Database failed",//70
    "Copy ROI Functionality is failed",//71
    "Getting Scanned ProtocolList failed",//72
	"Validation failed. Please check error log.", //73
    "Phase correction failed Please check error log for details", //74
    "Study files not found for VFC operations",//75
	"Failed to update in VFC", //76 
    "Raw Data Checking Failed. Refer Error Log" //77
};

//}

#ifndef __PowerSupplyMonitorCalibration_h__
#define __PowerSupplyMonitorCalibration_h__

#define MAX_WATTS    30.0F   // Fan is highest at this
#define MAX_AMPS      2.0F   // Fan is highest at this


// Calibration procedure
   
/*  Voltage Reading Calibration
 *  
 *  1) Connect a multi meter to the unit.  Voltage should ideally be around 19V
 *     assuming your multimeter has a 0 - 20V range, or the PSUs highest setting. 
 *  2) Adjust VOLT_SET_MULTIPLIER and re-flash until the multi meter and display match.
 *  3) Set a low voltage.  The lower the better as long as it is stable. Lower your
 *     multi meter range accordingly for better accuracy
 *  4) Adjust VOLT_SET_OFFSET and re-flash until the multi meter and display match.
 *  5) Repeat all steps until no adjustmets are required.
 */
#define VOLT_SET_MULTIPLIER       11.103254F       // Multiply ADS reading my this to get input voltage
#define VOLT_SET_OFFSET           -0.008F         // Add to voltage reading to correct any offset

/*  Actual Amp Reading Calibration
 *  
 *  1) Connect multi meter between PSU and a load to to read the actual amps being drawn.
 *     set multi meter to a range that can measure max amps or better
 *  2) Connect a load capable of pulling a steady load near max amps.
 *  3) Turn on the load for a moment and note the difference in readings between 
 *     the load and the multi meter. 
 *  4) Make an adjustment to SHUNT_RESISTOR and re-flash
 *  5) Repeat steps 3 & 4 until the displays match
 *  8) Connect a load capable of pulling a steady load of just a few milli amps.
 *  9) Make an adjustment to SHUNT_OFFSET and re-flash
 *  10) Repeat steps 6 & 7 until the displays match
 *  11) Repeat all steps until no adjustmets are required. Repeat in other 
 *      multi meter ranges to cross check. The high amp range may not be very
 *      accurate
 */
#define SHUNT_RESISTOR              .47394F         // Shunt resistor in ohms
#define SHUNT_OFFSET                -.0038F         // Add to amp reading to correct any offset

/*  Current Limit Amp Reading Calibration
 *   (Multi meter not needed)
 *  
 *  1) Adjust the current limit pot to a high value.
 *  2) Connect a load capable of pulling a steady load above the current limit.
 *  3) Turn on the load for a moment and note the difference between the amp reading
 *     and the current limit.
 *  4) Make an adjustment to AMP_SET_ADJUST and re-flash
 *  5) Repeat steps 3 & 4 until the displays match
 *  6) Adjust the current limit to near the lowest setting
 *  7) Turn on the load for a moment and note the difference between the amp reading
 *     and the current limit.
 *  8) Make an adjustment to AMP_SET_OFFSET and re-flash
 *  9) Repeat steps 7 & 8 until the displays match
 *  10) Repeat all steps until no adjustmets are required.
 */
#define AMP_SET_ADJUST                2.0897F      
#define AMP_SET_OFFSET               -0.0063F        // Add to amp set reading to correct any offset

/* Fixing then the current limit setting display when the displayed value changes when a load is applied
 *  
 *  1) Under no load, Set the current adjust to 1.000 amps
 *  2) Short the output, or use some other method of creating a load of > 1 amp
 *  3) Set AMP_SET_LOAD_COMPENSATION to how much the current limi display decreases under load
 *  4) Test and make small adjustments to AMP_SET_LOAD_COMPENSATION until the reading does not change when under load.
 */

#define AMP_SET_LOAD_COMPENSATION    0.0029F
 
#endif


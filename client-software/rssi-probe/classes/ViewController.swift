import UIKit
import CoreBluetooth

class ViewController: UIViewController, CBCentralManagerDelegate {
    @IBOutlet private var scanningSwitch: UISwitch?
    @IBOutlet private var loggingButton: UIButton?
    @IBOutlet private var nodeLabelOne: UILabel?
    @IBOutlet private var nodeLabelTwo: UILabel?
    @IBOutlet private var nodeLabelThree: UILabel?
    
    private let numberForUUID: [String: UInt8] = ["25FC9103-DFA9-2FC0-9E26-36739E77A391": 1,
                                                   "2F51A3B5-7657-D122-3960-B3DA5F55F912": 2,
                                                   "6FBDD05B-D61E-0F5B-2AB4-A3FF5B4A3315": 3]

    private var centralManager: CBCentralManager?

    private var scanning = false
    private var logging = false
    //Key is a device UUID, stores a tuple of the date and the RSSI
    private var log = Dictionary<String, [(date: NSDate, rssi: Int)]>()
    //Our central will only report data from devices broadcasting this service
    //in their advertising data
    private let permissibaleUUID =  CBUUID.UUIDWithString("FFE0")
    
    private let dateFormatString = "MM-dd-yyyy HHmmss"
    
    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        commonInit()
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        commonInit()
    }
    override func viewDidAppear(animated: Bool) {
        loggingButton!.enabled = false
    }
    func commonInit(){
        let options = [CBCentralManagerScanOptionAllowDuplicatesKey : true]
        centralManager = CBCentralManager(delegate: self, queue: nil, options: options)
        
    }
    @IBAction func scanningSwitchFlipped(sender: UISwitch){

        if scanning{
            stopScanning()
            //Simulate a tap on the logging button to wind that stuff down
            if logging{
                loggingButtonPressed(loggingButton!)
            }
            loggingButton!.enabled = false
            nodeLabelOne!.text = "Node One"
            nodeLabelTwo!.text = "Node Two"
            nodeLabelThree!.text = "Node Three"
            scanning = false
            UIApplication.sharedApplication().idleTimerDisabled = false
        }
        else{
            scanning = true
            UIApplication.sharedApplication().idleTimerDisabled = true
            loggingButton!.enabled = true
            let options = [CBCentralManagerScanOptionAllowDuplicatesKey : true]
            centralManager!.scanForPeripheralsWithServices([permissibaleUUID], options: options)
        }
    }
    @IBAction func loggingButtonPressed(sender: UIButton){
        
        if !logging{
            //May want to put a timeout here in the future
            //NSTimer.scheduledTimerWithTimeInterval(7200.0, 
                //target: self, selector: Selector("stopScanning"), userInfo: nil, repeats: false)
            println("Started logging.")
            logging = true
            //Relabel the button
            sender.setTitle("Stop logging", forState: nil)
        }
        else{
            //Relabel the button
            sender.setTitle("Start logging", forState: nil)
            println("Stopped logging.")
            promptToNameLogFile()
            logging = false
        }

    }
    func updateLabel(uuid: String){
        let average = getAverageOfLastTenEvents(uuid)
        let peripheralNumber = numberForUUID[uuid]
        var labelForPeripheral: UILabel?
        if peripheralNumber != nil {
            switch peripheralNumber! {
            case 1: labelForPeripheral = nodeLabelOne
            case 2: labelForPeripheral = nodeLabelTwo
            case 3: labelForPeripheral = nodeLabelThree
            default:
                println("Woops")
            }
        }
        if labelForPeripheral != nil {
            labelForPeripheral!.text = average!.description
        }
    }
    func centralManager(central: CBCentralManager!, didDiscoverPeripheral peripheral: CBPeripheral!,
        advertisementData: [NSObject : AnyObject]!, RSSI: NSNumber!) {
        //The TI BLE stack uses this to indicate a hardware error in retrieving the RSSI.
        //Usually caused by advertising faster than the information can be surfaced.
        if RSSI == 127 {
            //We'll just ignore this data.
            return
        }
        let timeStamp = NSDate()
        let uuid = peripheral.identifier.UUIDString
        let rssi = RSSI as Int

        //If the log array doesn't exist for this device,
        //make it.
        if log[uuid]? == nil{
            log[uuid] = []
        }

        //See if the user wants us to log stuff
        if logging{
            //Add an event
            log[uuid]!.append(date: timeStamp as NSDate, rssi: rssi)
        }
        else{
            //We'll just keep 10 events at a time so we
            //can keep the meters updating.
            logEventUpToTen(uuid, timeStamp: timeStamp, rssi: rssi)
        }

        updateLabel(uuid)
        
    }
    func centralManagerDidUpdateState(central: CBCentralManager!) {
        //Required to implement the protocol. Not needed for us.
    }
    func stopScanning(){
        centralManager!.stopScan()
    }
    //MARK: Logging

    func logEventUpToTen(uuid: String, timeStamp: NSDate, rssi: Int){
        let arrayForUUID = log[uuid]

        if arrayForUUID != nil{
            if arrayForUUID!.count == 10 {
                //Rotate all data back one index

                for var i = 0; i < 9; i++ {
                    log[uuid]![i] = log[uuid]![i + 1]
                }
                log[uuid]![9] = (timeStamp,rssi)
            }
            //If we've got fewer than ten, just add our new event on
            else if arrayForUUID!.count < 10{
                log[uuid]!.append(date: timeStamp, rssi: rssi)
            }
            else{
                println("method called with more than ten elements already in the log")
            }

        }
    }

    func getAverageOfLastTenEvents(uuid: String)->(Int8?){
        var arrayForUUID = log[uuid]
        //Make sure we've actually logged something for this ID
        if arrayForUUID != nil{
            if arrayForUUID!.count > 10 {
                //...get the average of all the events
                var totalOfLastTenMeasurements = 0
                let lastIndex = arrayForUUID!.count - 1

                for var i = 0; i < 10; i++ {

                    totalOfLastTenMeasurements += arrayForUUID![lastIndex - i].rssi
                }
                var average = totalOfLastTenMeasurements / 10
                return Int8(average)
            }
            //If we've got fewer than ten, just return one of them
            else if arrayForUUID!.count > 0 {
                return Int8(arrayForUUID!.last!.rssi)
            }
            else {
                println("Uh oh, there's nothing in the array...")
                return nil
            }
        }
        return nil
        
    }
    func getDocumentsDirectory()->String{
        return NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)[0] as String
    }

    func logToString()->(String){
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = dateFormatString
        var logString = ""
        for (uuid, events) in log {
            logString += "\(uuid)\n";
            for logEvent in events{
                logString += "\(dateFormatter.stringFromDate(logEvent.date)) \(logEvent.rssi) \n"
            }
        }
        return logString
    }

    func logToStringWithoutTime()->(String){
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = dateFormatString
        var logString = ""
        for (uuid, events) in log {
            logString += "\n \(uuid)";
            for logEvent in events{
                logString += ", \(logEvent.rssi)"
            }
        }
        return logString
    }
    func promptToNameLogFile(){
        var alert = UIAlertController(title: "Log Name",
            message: "Enter a name to describe what was just logged",
            preferredStyle: UIAlertControllerStyle.Alert)

        alert.addTextFieldWithConfigurationHandler({ (textField) in
            textField.placeholder = "Name"
        })
        alert.addAction(UIAlertAction(title: "OK", style: .Default, handler:{ (alertAction:UIAlertAction!) in
            let field = alert.textFields![0] as UITextField
            self.saveLogToFile(field.text)
        }))

        self.presentViewController(alert, animated: true, completion: nil)
    }

    func saveLogToFile(name: String){
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = dateFormatString
        let logString = logToStringWithoutTime()
        println(logString)
        let documentsDirectory = getDocumentsDirectory()
        var fileName = dateFormatter.stringFromDate(NSDate.date())
        fileName += name
        fileName += ".txt"
        let path = documentsDirectory.stringByAppendingPathComponent(fileName)
        println(path)
        logString.writeToFile(path, atomically: true, encoding: NSUTF8StringEncoding, error: nil)
        
        log = Dictionary<String, [(date: NSDate, rssi: Int)]>()
    }


}


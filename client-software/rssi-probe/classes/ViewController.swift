import UIKit
import CoreBluetooth

class ViewController: UIViewController, CBCentralManagerDelegate {
    @IBOutlet private var scanningSwitch: UISwitch?
    @IBOutlet private var loggingButton: UIButton?
    @IBOutlet private var nodeLabelOne: UILabel?
    @IBOutlet private var nodeLabelTwo: UILabel?
    @IBOutlet private var nodeLabelThree: UILabel?
    @IBOutlet private var maxLogCountLabel: UILabel?
    @IBOutlet private var logProgressIndicator: UIProgressView?

    private let numberForUUID: [String:UInt8] = ["5637417B-E93B-DE78-45F5-B7888477AAEF": 1,
                                                 "2F51A3B5-7657-D122-3960-B3DA5F55F912": 2,
                                                 "6FBDD05B-D61E-0F5B-2AB4-A3FF5B4A3315": 3]

    var centralManager: CBCentralManager?

    private var scanning = false
    private var logging = false
    private var log = Log()
    private var buffers = [String: AverageBuffer]()
    //Our central will only report data from devices broadcasting this service
    //in their advertising data
    private let permissibaleUUID = CBUUID(string: "FFE0")

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

    func commonInit() {
        let options = [CBCentralManagerScanOptionAllowDuplicatesKey: true]
        centralManager = CBCentralManager(delegate: self, queue: nil, options: options)

    }

    @IBAction func scanningSwitchFlipped(sender: UISwitch) {

        if scanning {
            stopScanning()
            //Simulate a tap on the logging button to wind that stuff down
            if logging {
                loggingButtonPressed(loggingButton!)
            }
            loggingButton!.enabled = false
            nodeLabelOne!.text = "Node One"
            nodeLabelTwo!.text = "Node Two"
            nodeLabelThree!.text = "Node Three"
            scanning = false
            UIApplication.sharedApplication().idleTimerDisabled = false
        } else {
            scanning = true
            UIApplication.sharedApplication().idleTimerDisabled = true
            loggingButton!.enabled = true
            let options = [CBCentralManagerScanOptionAllowDuplicatesKey: true]
            centralManager!.scanForPeripheralsWithServices([permissibaleUUID], options: options)
        }
    }

    @IBAction func loggingButtonPressed(sender: UIButton) {
        if !logging {
            //May want to put a timeout here in the future
            //NSTimer.scheduledTimerWithTimeInterval(7200.0,
            //target: self, selector: Selector("stopScanning"), userInfo: nil, repeats: false)
            println("Started logging.")
            logging = true
            //Relabel the button
            sender.setTitle("Stop logging", forState: nil)
        } else {
            //Relabel the button
            sender.setTitle("Start logging", forState: nil)
            logging = false
            println("Stopped logging.")
            promptToNameLogFile()

        }

    }

    func updateLabel(uuid: String) {
        var average: Int?
        if scanning || logging {
            let buffer = buffers[uuid]
            if buffer != nil {
                average = Int(buffer!.average!)
            }
        }
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
        let max = log.getMaxNumberOfEntries()

        maxLogCountLabel!.text = max.description
        logProgressIndicator!.progress = Float(max) / 400.0
        logProgressIndicator!.setNeedsDisplay()
    }

    func centralManager(central: CBCentralManager!, didDiscoverPeripheral peripheral: CBPeripheral!,
                        advertisementData: [NSObject:AnyObject]!, RSSI: NSNumber!) {
        //The TI BLE stack uses this to indicate a hardware error in retrieving the RSSI.
        //Usually caused by advertising faster than the information can be surfaced.
        if RSSI == 127 {
            //We'll just ignore this data.
            return
        }
        let timeStamp = NSDate()
        let uuid = peripheral.identifier.UUIDString
        let rssi = RSSI as Int

        if buffers[uuid] == nil {
            buffers[uuid] = AverageBuffer(bufferSize: 10)
        }
        buffers[uuid]!.add(rssi)
        println(rssi)
        //See if the user wants us to log stuff
        if logging {
            //Add an event
            log.addToLog(uuid, timeStamp: timeStamp as NSDate, rssi: rssi)

            if log.getMaxNumberOfEntries() > 399 {
                loggingButtonPressed(loggingButton!)
            }
        }

        updateLabel(uuid)

    }
    /**
    Show an alert with a textfield and ask the user to name the file or discard it.
    */
    func promptToNameLogFile() {
        var alert = UIAlertController(title: "Log Name",
                                      message: "Enter a name to describe what was just logged",
                                      preferredStyle: UIAlertControllerStyle.Alert)

        alert.addTextFieldWithConfigurationHandler({
            (textField) in
            textField.placeholder = "Name"
        })
        alert.addAction(UIAlertAction(title: "OK", style: .Default, handler: {
            (alertAction: UIAlertAction!) in
            let field = alert.textFields![0] as UITextField
            self.log.saveLogToFile(field.text)
        }))
        alert.addAction(UIAlertAction(title: "Discard", style: .Destructive, handler: {
            (alertAction: UIAlertAction!) in
            let field = alert.textFields![0] as UITextField
            self.log.clearLog()
        }))

        self.presentViewController(alert, animated: true, completion: nil)
    }

    func centralManagerDidUpdateState(central: CBCentralManager!) {
        //Required to implement the protocol. Not needed for us.
    }

    func stopScanning() {
        centralManager!.stopScan()
    }

}


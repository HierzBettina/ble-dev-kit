import Foundation
import UIKit

typealias LogEntry = (date:NSDate, rssi:Int)


class Log {
    //Key is a device UUID, stores a tuple of the date and the RSSI
    private var log = Dictionary<String, [LogEntry]>()
    private let dateFormatString = "MM-dd-yyyy HHmmss"

    func getDocumentsDirectory() -> String {
        return NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)[0] as String
    }

    func logToString() -> (String) {
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = dateFormatString
        var logString = ""
        for (uuid, events) in log {
            logString += "\(uuid)\n";
            for logEvent in events {
                logString += "\(dateFormatter.stringFromDate(logEvent.date)) \(logEvent.rssi) \n"
            }
        }
        return logString
    }

    func logToCSVStringWithoutTime() -> (String) {
        var rows: [String] = []

        for (uuid, events) in log {
            //Add the uuid as the column header
            if 0 == rows.endIndex {
                rows.append("\(uuid),")
            } else {
                rows[0] += "\(uuid),"
            }
            for var i = 0; i < events.count; i++ {
                if i + 1 < rows.count {
                    rows[i + 1] += "\(events[i].rssi.description),"
                } else {
                    rows.append("\(events[i].rssi.description),")
                }
            }
        }

        var logString = ""
        for row in rows {
            logString += "\(row.substringToIndex(row.endIndex.predecessor()))\n"
        }
        return logString
    }

    func getMaxNumberOfEntries() -> Int {
        var max = 0;
        for (uuid, events) in log {
            max = (events.count > max) ? events.count : max
        }
        return max;
    }

    func addToLog(uuid: String, timeStamp: NSDate, rssi: Int) {
        //If the log array doesn't exist for this device,
        //make it.
        if log[uuid] == nil {
            log[uuid] = []
        }
        let arrayForUUID = log[uuid]
        log[uuid]!.append(date: timeStamp, rssi: rssi)

    }

    func saveLogToFile(name: String) {
        let dateFormatter = NSDateFormatter()
        dateFormatter.dateFormat = dateFormatString
        let logString = logToCSVStringWithoutTime()
        println(logString)
        let documentsDirectory = getDocumentsDirectory()
        var fileName = dateFormatter.stringFromDate(NSDate())
        fileName += " \(name).csv"
        let path = documentsDirectory.stringByAppendingPathComponent(fileName)
        println(path)
        logString.writeToFile(path, atomically: true, encoding: NSUTF8StringEncoding, error: nil)

        clearLog()
    }

    func clearLog() {
        log = Dictionary<String, [LogEntry]>()
    }
}
import rssiprobe
import UIKit
import XCTest

class AverageBufferTests: XCTestCase {
    private var buffer = AverageBuffer(bufferSize: 10)
    override func setUp() {
        super.setUp()
        buffer = AverageBuffer(bufferSize: 10)
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }

    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }

    func testAverage() {
        XCTAssertNil(buffer.average, "Average should be nil when buffer is empty")
        buffer.add(100)
        XCTAssertEqual(buffer.average!, 100.0, "Average should be 50.0")
        buffer.add(50)
        XCTAssertEqual(buffer.average!.distanceTo(75.0), 0)
        buffer.add(100)
        XCTAssertLessThan(buffer.average!.distanceTo(250.0 / 3), 0.0001)
    }

    func testAveragingZero() {
        buffer.add(100)
        buffer.add(10)
        buffer.add(0)
        XCTAssertEqual(0.0, buffer.average!.distanceTo(110.0 / 3.0))
    }

    func testNegativeAveraging() {
        buffer.add(-60)
        buffer.add(-50)
        buffer.add(-40)
        XCTAssertEqual(buffer.average!, -50.0)
    }

    func testContinuousBuffering() {
        for var i = 1; i < 11; i++ {
            buffer.add(i)
        }
        //Buffer should average 89...99
        XCTAssertLessThan(buffer.average!.distanceTo(5.5), 0.00001)
    }


    func testBufferRemovingExcess() {
        for var i = 0; i < 10; i++ {
            buffer.add(0)
        }
        buffer.add(10)
        XCTAssertEqual(buffer.average!, 1.0, "Buffer should remove a zero and add a ten")

        for var i = 0; i < 9; i++ {
            buffer.add(10)
        }

        XCTAssertEqual(buffer.average!, 10.0, "Buffer should remove all zero and have all tens")

        for var i = 0; i < 10; i++ {
            buffer.add(0)
        }
        XCTAssertEqual(buffer.average!, 0.0, "Buffer should remove all tens")

    }
}

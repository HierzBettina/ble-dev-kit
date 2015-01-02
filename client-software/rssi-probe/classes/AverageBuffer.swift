import Foundation

//Add to the front. Supports up to constant limit, then removes from the back.
//Once it contains bufferSize elements, it is guranteed to contain exactly bufferSize items for the rest of its life
//Supports fast averaging

public class AverageBuffer {
    let bufferSize: Int
    private var list = LinkedList<Int>()
    public var average: Double?

    public init(bufferSize: Int) {
        self.bufferSize = bufferSize
    }

    public func add(value: Int) {
        list.prepend(value)
        if list.size == 1 {
            average = Double(list.head!.value)
        } else if list.size > bufferSize {
            let removed = list.removeLast()!
            let change = Double(removed.value) / Double(bufferSize)
            average! -= change
            average! += Double(value) / Double(bufferSize)
        } else if list.size <= bufferSize {
            let oldComponent = average! * Double(list.size - 1) / Double(list.size)
            let newComponent = Double(value) / Double(list.size)
            average = oldComponent + newComponent
        }


    }
}

private class LinkedList<V> {
    var head: LinkedListNode<V>?
    private var tail: LinkedListNode<V>?
    var size: Int = 0

    init() {

    }

    init(value: V) {
        //Prepend, append, for the first, it doesn't matter
        append(value)
    }

    func prepend(value: V) {
        size++
        let nextNode = head
        head = LinkedListNode<V>(value: value, next: nextNode, prev: nil)
        handleSingleton()
    }

    func append(value: V) {
        size++
        let prevNode = tail
        tail = LinkedListNode<V>(value: value, next: nil, prev: prevNode)

        handleSingleton()
    }

    func removeLast() -> (LinkedListNode<V>?) {
        size--
        let removed = tail
        tail = removed!.prev
        tail!.next = nil
        handleSingleton()
        return removed
    }

    func removeFirst() -> (LinkedListNode<V>?) {
        size--
        let removed = head
        head = removed!.next
        head!.prev = nil
        handleSingleton()
        return removed
    }

    private func handleSingleton() {
        if size == 1 {
            if tail != nil {
                head = tail
            } else {
                tail = head
            }
        }
    }
}

private class LinkedListNode<V> {
    var value: V
    var next: LinkedListNode?
    var prev: LinkedListNode?
    init(value: V, next: LinkedListNode?, prev: LinkedListNode?) {
        self.value = value
        self.next = next
        self.prev = prev
        if next != nil {
            self.next!.prev = self
        }
        if prev != nil {
            self.prev!.next = self
        }
    }
}
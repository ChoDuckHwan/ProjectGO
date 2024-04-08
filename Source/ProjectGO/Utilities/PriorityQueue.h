template <typename InElementType>
struct TPriorityQueueNode {
    InElementType Element;
    int Priority;

    TPriorityQueueNode()
    {
    }

    TPriorityQueueNode(InElementType InElement, int InPriority)
    {
        Element = InElement;
        Priority = InPriority;
    }

    bool operator<(const TPriorityQueueNode<InElementType> Other) const
    {
        return Priority < Other.Priority;
    }
};

template <typename InElementType>
class TPriorityQueue {
public:
    TPriorityQueue()
    {
        Array.Heapify();
    }

public:
    // Always check if IsEmpty() before Pop-ing!
    InElementType Pop()
    {
        TPriorityQueueNode<InElementType> Node;
        Array.HeapPop(Node);
        return Node.Element;
    }

    TPriorityQueueNode<InElementType> PopNode()
    {
        TPriorityQueueNode<InElementType> Node;
        Array.HeapPop(Node);
        return Node;
    }

    void Push(InElementType Element, float Priority)
    {
        Array.HeapPush(TPriorityQueueNode<InElementType>(Element, Priority));
		Array.HeapSort();
    }

    bool IsEmpty() const
    {
        return Array.Num() == 0;
    }

private:
    TArray<TPriorityQueueNode<InElementType>> Array;
};
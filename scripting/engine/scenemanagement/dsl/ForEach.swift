public func ForEach<Content: SceneNode>(
    _ nodes: [Content], 
    @SceneGroupBuilder content: (Content) -> SceneNodeCollection
) -> SceneNodeCollection {
    SceneNodeCollection(nodes: nodes.flatMap { content($0).nodes })
}

public func ForEach(
    _ range: Range<Int>,
    _block: @escaping (Int) -> SceneNode
) -> SceneGroup {
    var nodes: [SceneNode] = []
    
    range.forEach { value in
        nodes.append(_block(value))
    }

    return SceneGroup(collection: SceneNodeCollection(nodes: nodes))
}
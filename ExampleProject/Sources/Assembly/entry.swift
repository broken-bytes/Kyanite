import GameKit

var instance: GameKit!

@_cdecl("GameKitMain") public func GameKitMain() -> Void {
    print("Creating instance")
    instance = GameKit()
    print("Instance created")
}
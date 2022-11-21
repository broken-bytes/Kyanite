import Audio
import Foundation

public struct AudioComponent: Component {
    public var emitter: AudioEmitter

    public init() {
        emitter = AudioEmitter(currentClip: AudioClip(data: Data(), length: 0), currentTime: 0)
    }
}
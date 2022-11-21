import Foundation
#if os(macOS)
import AVFoundation
#endif

public class AudioPlayer {
    public static let shared = AudioPlayer()

    private init() {

    }

    public func play(clip: AudioClip, begin at: Int) {
        
    }
}
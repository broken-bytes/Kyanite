public class AudioEmitter {
    public var currentClip: AudioClip
    public var currentTime: Int

    public var isPlaying: Bool

    public init(currentClip: AudioClip, currentTime: Int) {
        self.currentClip = currentClip
        self.currentTime = currentTime
        self.isPlaying = false
    }
}
import Userland

func initUserCode() {
    let c = PlayerComponent(id: 0)
}

#if _EDITOR

@_cdecl("assemblyInit") public func assemblyInit(nativeCore: UnsafeMutableRawPointer) {
    initUserCode()
}

#else


@_cdecl("assemblyInit") public func assemblyInit() {
    initUserCode()
}

#endif
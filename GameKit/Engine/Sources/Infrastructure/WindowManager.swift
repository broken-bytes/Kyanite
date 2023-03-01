import AppKit

class App : NSObject, NSApplicationDelegate {
    let app = NSApplication.shared
    let name = ProcessInfo.processInfo.processName
    let status = NSStatusBar.system.statusItem(withLength: NSStatusItem.variableLength)
    let window = NSWindow.init(
        contentRect: NSMakeRect(0, 0, 200, 200),
        styleMask: [.titled, .closable, .miniaturizable],
        backing: .buffered,
        defer: false
    )

    override init() {
        super.init()
        app.setActivationPolicy(.accessory)

        window.center()
        window.title = name
        window.hidesOnDeactivate = false
        window.isReleasedWhenClosed = false

        let statusMenu = newMenu()
        status.button?.title = "🤓"
        status.menu = statusMenu

        let appMenu = newMenu()
        let sub = NSMenuItem()
        sub.submenu = appMenu
        app.mainMenu = NSMenu()
        app.mainMenu?.addItem(sub)
    }
}
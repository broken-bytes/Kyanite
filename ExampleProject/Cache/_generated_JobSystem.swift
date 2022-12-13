import EntityComponentSystem
import Profiling
import Foundation
import Userland

var jobs: [Job] = []

@_cdecl("AssemblyJobsInit") func initJobSystem() {

}

@_cdecl("AssemblyJobsUpdate") func executeJobs() {
    
}
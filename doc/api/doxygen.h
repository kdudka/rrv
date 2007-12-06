// This file contains no code inside
/*
 * @file doxygen.h
 * @brief Doxygen documentation (common part)
 * @author Kamil Dudka, xdudka00@gmail.com
 * @date 2007-05-15
 */

/*
 * @mainpage
 * @brief Library for work with objects in shared memory
 * @author Kamil Dudka, xdudka00@gmail.com
 * @date 2007-05-15
 * @remarks
 * @htmlonly
 * <p>You can see overview in <a class="el" href="modules.html">Modules</a> section.</p>
 * <p>Command-line utility for manipulating with shared segment: <a class="el" href="group__sharectl.html#_details">sharectl</a></p>
 * <p>Quick links to library fundamental classes:</p>
 * <ul>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1SharedObject.html">SharedObject</a> - shareable object base class</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1RelocPtr.html">RelocPtr</a> - relocable pointer type</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1ShareManager.html">ShareManager</a> - Gateway to shared segment</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1Allocator.html">Allocator</a> - STL allocator</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1Vector.html">Vector</a> - STL vector</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1String.html">String</a> - STL string</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1Map.html">Map</a> - STL map</li>
 * </ul>
 * @endhtmlonly
 */

/*
 * @defgroup core Core library classes
 * @brief Library interface - core functionality
 * @remarks
 * @htmlonly
 * <ul>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1SharedObject.html">SharedObject</a> - shareable object base class</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1RelocPtr.html">RelocPtr</a> - relocable pointer type</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1ShareManager.html">ShareManager</a> - Gateway to shared segment</li>
 * <li style="margin-bottom:1em;"><a class="el" href="classShare_1_1Allocator.html">Allocator</a> - STL allocator</li>
 * </ul>
 * @endhtmlonly
 */

/*
 * @htmlonly
 * <h3>Usage</h3>
 * <p style="margin-left:2em;"><code>sharectl SEGMENT_NAME [SWITCH] [...]</code></p>
 * <table style="margin-left:4em;">
 * <tr><td style="font-family:monospace">--create</td><td style="padding-left:2em;">Create new shared segment of specified size.</td></tr>
 * <tr><td style="font-family:monospace">--create-and-wait</td><td style="padding-left:2em;">Create new shared segment of specified size and wait until key pressed.</td></tr>
 * <tr><td style="font-family:monospace">--notify-attach</td><td style="padding-left:2em;">Increase internal reference counter.</td></tr>
 * <tr><td style="font-family:monospace">--notify-detach</td><td style="padding-left:2em;">Decrease internal reference counter.</td></tr>
 * <tr><td style="font-family:monospace">--unlink</td><td style="padding-left:2em;">Mark segment to be destroyed.</td></tr>
 * <tr><td style="font-family:monospace">--force-destroy</td><td style="padding-left:2em;">Destroy segment immediately.</td></tr>
 * <tr><td style="font-family:monospace">--alloc</td><td style="padding-left:2em;">Allocate block of specified size inside shared segment.</td></tr>
 * <tr><td style="font-family:monospace">--free</td><td style="padding-left:2em;">Free block identified by relative pointer (number)</td></tr>
 * </table>
 * @endhtmlonly
 * @defgroup sharectl sharectl utility
 * @brief @c sharectl is command-line utility for manipulating with shared segment.
 */

/*
 * @defgroup backend Internal library classes
 */

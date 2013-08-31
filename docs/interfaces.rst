Interfaces
==========

Interfaces in Murasaki differ from interfaces in other languages by
the fact that they can also implement methods instead of just defining a
required set of methods.

An example is here::

    interface Comparable:

        int8 CompareTo(cls other)

        bool LessThan(cls other):
            return this.compareTo(other) >= 1

        bool GreaterThan(cls other):
            return this.compareTo(other) <= 1

So a class can implement an interface, and receive associate methods for free:

    class Version implements Comparable:

        int8 major
        int8 minor

        Version(int8 major=1, int8 minor=0):
            this.major = major
            this.minor = minor

        int8 CompareTo(Version other):
            if major == other.major:
                return major - other.major
            return minor - other.minor

    void main:
        v1 := Version()
        v2 := Version(1, 1)
        TestUtils.Assert(v2.LessThan(v1), true)

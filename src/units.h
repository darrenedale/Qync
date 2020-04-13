/**
 * @file units.h
 *
 * @brief Definition of user-defined literals for digital sizes.
 *
 * The following user-defined literal types are provided, for both unsigned long
 * long (i.e. integer) and long double (i.e. floating point) literals:
 * - @b _gib (IEC Gibibytes, 1024 Mibibytes)
 * - @b _mib (IEC Mibibytes, 1024 Kibibytes)
 * - @b _kib (IEC Kibibytes, 1024 bytes)
 * - @b _gb (SI Gigabytes, 1000 Megabytes)
 * - @b _mb (SI Megabytes, 1000 Kilobytes)
 * - @b _kb (SI Kilobytes, 1000 bytes)
 * - @b _b (bytes, same for both IEC and SI)
 *
 * The user-defined literals are available only in the Qync namespace.
 */

#ifndef QYNC_UNITS_H
#define QYNC_UNITS_H

namespace Qync {
	/** @brief Enable expression of floating-point sizes in IEC Gib. */
	constexpr long double operator"" _gib(long double gib) {
		/* 1 gib = 1024mib; 1mib = 1024kib; 1kib = 1024 bytes
		 * compiler should optimise this so keep it long-form for clarity */
		return gib * 1024.0L * 1024.0L * 1024.0L;
	}

	/** @brief Enable expression of integral sizes in IEC Gib. */
	constexpr unsigned long long operator"" _gib(unsigned long long gib) {
		/* 1 gib = 1024mib; 1mib = 1024kib; 1kib = 1024 bytes
		 * compiler should optimise this so keep it long-form for clarity */
		return gib * 1024ULL * 1024ULL * 1024ULL;
	}

	/** @brief Enable expression of floating-point sizes in SI Gb. */
	constexpr long double operator"" _gb(long double gb) {
		/* 1 gb = 1000mb; 1mb = 1000kb; 1kb = 1000 bytes
		 * compiler should optimise this, so keep it long-form for clarity */
		return gb * 1000.0L * 1000.0L * 1000.0L;
	}

	/** @brief Enable expression of integral sizes in SI Gb. */
	constexpr unsigned long long operator"" _gb(unsigned long long gb) {
		/* 1 gb = 1000mb; 1mb = 1000kb; 1kb = 1000 bytes
		 * compiler should optimise this, so keep it long-form for clarity */
		return gb * 1000ULL * 1000ULL * 1000ULL;
	}

	/** @brief Enable expression of floating-point sizes in IEC Mib. */
	constexpr long double operator"" _mib(long double mib) {
		/* 1mib = 1024kib; 1kib = 1024 bytes
		 * compiler should optimise this so keep it long-form for clarity */
		return mib * 1024.0L * 1024.0L;
	}

	/** @brief Enable expression of integral sizes in IEC Mib. */
	constexpr unsigned long long operator"" _mib(unsigned long long mib) {
		/* 1mib = 1024kib; 1kib = 1024 bytes
		 * compiler should optimise this so keep it long-form for clarity */
		return mib * 1024ULL * 1024ULL;
	}

	/** @brief Enable expression of floating-point sizes in SI Mb. */
	constexpr long double operator"" _mb(long double mb) {
		/* 1mb = 1000kb; 1kb = 1000 bytes
		 * compiler should optimise this, so keep it long-form for clarity */
		return mb * 1000.0L * 1000.0L;
	}

	/** @brief Enable expression of integral sizes in SI Mb. */
	constexpr unsigned long long operator"" _mb(unsigned long long mb) {
		/* 1mb = 1000kb; 1kb = 1000 bytes
		 * compiler should optimise this, so keep it long-form for clarity */
		return mb * 1000ULL * 1000ULL;
	}

	/** @brief Enable expression of floating-point sizes in IEC Kib. */
	constexpr long double operator"" _kib(long double kib) {
		/* 1kib = 1024 bytes
		 * compiler should optimise this so keep it long-form for clarity */
		return kib * 1024.0L;
	}

	/** @brief Enable expression of integral sizes in IEC Kib. */
	constexpr unsigned long long operator"" _kib(unsigned long long kib) {
		/* 1kib = 1024 bytes
		 * compiler should optimise this so keep it long-form for clarity */
		return kib * 1024ULL;
	}

	/** @brief Enable expression of floating-point sizes in SI Kb. */
	constexpr long double operator"" _kb(long double kb) {
		/* 1kb = 1000 bytes */
		return kb * 1000.0L;
	}

	/** @brief Enable expression of integral sizes in SI Kb. */
	constexpr unsigned long long operator"" _kb(unsigned long long kb) {
		/* 1kb = 1000 bytes */
		return kb * 1000ULL;
	}

	/**
	 * @brief Convenience literal to express a quantity in bytes.
	 *
	 * Literals of this type do not require any transformation of the quantity, and
	 * are the same for both SI and IEC unit systems. This UDL simply facilitates
	 * code expressiveness.
	 */
	constexpr unsigned long long operator"" _b(unsigned long long bytes) {
		return bytes;
	}

	/**
	 * @brief Convenience literal to express a quantity in bytes.
	 *
	 * Literals of this type do not require any transformation of the quantity, and
	 * are the same for both SI and IEC unit systems. This UDL simply facilitates
	 * code expressiveness.
	 */
	constexpr long double operator"" _b(long double bytes) {
		return bytes;
	}
}  // namespace Qync

#endif  // QYNC_UNITS_H

#ifndef PROGRESSSUBJECTMULTISTEP_H
#define PROGRESSSUBJECTMULTISTEP_H

/**
 * @file ProgressSubjectMultiStep.h
 * @brief Class ProgressSubjectMultiStep - observer's subject (base for RadiosityRenderer).
 * @author xdudka00
 * @date 2007-11-18
 */

#include <list>

class ProgressObserverMultiStep;

/**
 * @brief Observer's subject. Design pattern @b observer.
 */
class ProgressSubjectMultiStep
{
	public:
		virtual ~ProgressSubjectMultiStep() {}

		/**
		 * @brief Attach observer to subject
		 * @param observer Observer to attach.
		 * @param perStepFreq Frequency of callback during step.
		 */
		virtual void attach(ProgressObserverMultiStep *observer, int perStepFreq);

		/**
		 * @brief Detach observer from subject.
		 * @param observer Observer to detach.
		 */
		virtual void detach(ProgressObserverMultiStep *observer);

		/**
		 * @brief Return total count of steps.
		 */
		virtual int stepCount() const =0;

		/**
		 * @brief Return number of current step - always in range <0, stepCount()-1>.
		 */
		virtual int currentStep() const =0;
		
		/**
		 * @brief Return total count of patches.
		 */
		virtual int patchCount() const =0;

		/**
		 * @brief Return number of current patch - always in range <0, patchCount()-1>.
		 */
		virtual int currentPatch() const =0;
		
	protected:
		/**
		 * @brief Subject's notification about starting new step.
		 */
		void notifyStep();
		
		/**
		 * @brief Subject's notification about progress.
		 * @note This method should be called with enough frequency to acomplish
		 * observer's request.
		 */
		void notifyPerStepProgress();
	
	private:
		struct TItem {
			int offset;
			int ratio;
			int last;
			ProgressObserverMultiStep *observer;
		};
		typedef std::list<TItem> TList;
		TList observerList_;
		friend class ObserverFinder;
};

#endif // PROGRESSSUBJECTMULTISTEP_H
